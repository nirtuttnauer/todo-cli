#include "tasks/taskManager.h"

int main(int argc, char *argv[]) {
    // Run tests if needed
    // tests();

    // Validate argument count
    if (argc < 2 || argc > 4) {
        std::cerr << "Usage: task-cli <command> [argument] [extra]" << std::endl;
        return 1;
    }

    // Get the command
    std::string command = argv[1];

    // Handle optional argument
    std::string argument = (argc >= 3) ? argv[2] : "";
    std::string extra = (argc >= 4) ? argv[3] : "";


    // Initialize TaskManager
    try {
        TaskManager taskManager("tasks.json");
        taskManager.processCommand(command, argument, extra);
    } catch (const std::exception &error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}
