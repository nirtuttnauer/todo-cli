#include "utils/tasks/taskUtilities.h"
#include <ctime>

// Utility Functions
std::string currentTime()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);

    return std::string(buffer);
}

bool promptForConfirmation(const std::string &message)
{
    const std::string colorYellow = "\033[33m";
    const std::string colorReset = "\033[0m";

    std::cout << colorYellow << message << " (y/n): " << colorReset;
    std::string input;
    std::getline(std::cin, input);
    return input == "y" || input == "Y";
}

void printTask(const Task &task)
{
    const std::string colorReset = "\033[0m";
    const std::string colorCyan = "\033[36m";
    const std::string colorYellow = "\033[33m";
    const std::string colorGreen = "\033[32m";
    const std::string colorBlue = "\033[34m";
    const std::string colorGray = "\033[90m";

    std::string statusColor = colorReset;
    if (task.getStatus() == "todo")
        statusColor = colorYellow;
    else if (task.getStatus() == "in-progress")
        statusColor = colorGreen;
    else if (task.getStatus() == "done")
        statusColor = colorBlue;

    std::cout << colorCyan << "[" << task.getId() << "] " << colorReset
              << task.getDescription() << " (" << statusColor << task.getStatus() << colorReset << ") "
              << "| " << colorGray << "Created: " << task.getCreatedAt()
              << " | Updated: " << task.getUpdatedAt() << colorReset << std::endl;
}

