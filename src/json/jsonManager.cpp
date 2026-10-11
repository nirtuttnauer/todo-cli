#include "json/jsonManager.h"
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <limits>
#include <set>


jsonManager::jsonManager(const char *fileName) {
    // Get the user's Documents directory and append the invisible folder path
    const char *home = std::getenv("HOME"); // Get the home directory
    if (!home) {
        throw std::runtime_error("Unable to determine the home directory.");
    }
    std::filesystem::path folderPath = std::filesystem::path(home) / "Documents/.todo-cli";

    // Ensure the invisible folder exists
    if (!std::filesystem::exists(folderPath)) {
        std::filesystem::create_directories(folderPath);
    }

    // Set the JSON file path
    this->fileName = (folderPath / fileName).string();

    // Check if the JSON file exists, otherwise create it
    if (!std::filesystem::exists(this->fileName)) {
        create();
    }
}

void jsonManager::create() {
    std::ofstream outFile(this->fileName, std::ios::trunc);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + this->fileName);
    }
    outFile << "[]"; // Write an empty JSON array
    outFile.close();
}


// Keep the existing on-disk schema while using a complete JSON parser.
std::string jsonManager::tasksToJson(const std::vector<Task>& tasks) {
    auto json = nlohmann::json::array();
    for (const auto &task : tasks) {
        json.push_back({{"id", task.getId()}, {"description", task.getDescription()},
                        {"status", task.getStatus()}, {"createdAt", task.getCreatedAt()},
                        {"updatedAt", task.getUpdatedAt()}});
    }
    return json.dump();
}

std::vector<Task> jsonManager::jsonToTasks(const std::string& source) {
    const auto json = nlohmann::json::parse(source);
    if (!json.is_array()) {
        throw std::runtime_error("Task file must contain a JSON array.");
    }
    std::vector<Task> tasks;
    std::set<int> ids;
    for (const auto &item : json) {
        const auto &value = item.at("id");
        if (!value.is_number_integer() || value < 1 || value > std::numeric_limits<int>::max()) {
            throw std::runtime_error("Task IDs must be positive integers within range.");
        }
        const int id = value.get<int>();
        if (!ids.insert(id).second) {
            throw std::runtime_error("Duplicate task ID: " + std::to_string(id));
        }
        const auto status = item.at("status").get<std::string>();
        if (status != "todo" && status != "in-progress" && status != "done") {
            throw std::runtime_error("Invalid task status: " + status);
        }
        tasks.emplace_back(id, item.at("description").get<std::string>(), status,
                           item.at("createdAt").get<std::string>(),
                           item.at("updatedAt").get<std::string>());
    }
    return tasks;
}

// Save tasks to a file
void jsonManager::saveToFile(const std::vector<Task>& tasks) {
    // Serialize before opening a file, and replace only after a complete write.
    const auto data = tasksToJson(tasks);
    const auto temporary = this->fileName + ".tmp";
    std::ofstream outFile(temporary, std::ios::trunc);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + temporary);
    }
    outFile << data;
    outFile.close();
    if (!outFile) {
        std::filesystem::remove(temporary);
        throw std::runtime_error("Failed to write task file: " + this->fileName);
    }
    std::filesystem::rename(temporary, this->fileName);
}

std::vector<Task> jsonManager::loadFromFile() {
    std::ifstream inFile(this->fileName);  // Use 'this->fileName'
    if (!inFile.is_open()) {
        throw std::runtime_error("Failed to open file for reading: " + this->fileName);
    }

    std::ostringstream buffer;
    buffer << inFile.rdbuf();
    inFile.close();

    return jsonToTasks(buffer.str());
}
