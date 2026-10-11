#include "tasks/taskManager.h"
#include "utils/tasks/taskUtilities.h"

// Constructor and Destructor
TaskManager::TaskManager(const char *fileName) : fileName(fileName), jsonFile(fileName)
{
    load();
}

TaskManager::~TaskManager() = default;

// Command Processing
void TaskManager::processCommand(const std::string &command, const std::string &argument , const std::string &extra)
{
    if (command == "add" || command == "new" || command == "create" || command == "+")
    {
        addTask(argument);
    }
    else if (command == "list" || command == "ls" || command == "show")
    {
        listTasks(argument);
    }
    else if (command == "delete" || command == "del" || command == "remove" || command == "-" || command == "rm" || command == "destroy")
    {
        deleteTask(argument);
    }
    else if (command == "edit" || command == "update" || command == "change" || command == "modify" || command == "edit-description" || command == "change-description")
    {
        editTask(argument , extra);
    }
    else if (command == "mark-in-progress" || command == "mark-inprogress" || command == "mark-inprog" || command == "mark-in" || command == "mark-progress" || command == "mark-prog" || command == "mark-p" || command == "mi" || command == "mip")
    {
        markTaskAsInProgress(argument);
    }
    else if (command == "mark-done" || command == "mark-complete" || command == "mark-done-task" || command == "mark-complete-task" || command == "mark-completed-task" || command == "mark-completed" || command == "mark-comp" || command == "mark-c" || command == "mark-d" || command == "md" || command == "mc")
    {
        markTaskAsDone(argument);
    }
    else if (command == "unmark" || command == "um"){
        unmarkTask(argument);
    }
    else if (command == "status" || command == "s" || command == "st")
    {
        updateTaskStatus(argument);
    }
    else if (command == "clear" || command == "c" || command == "clr" || command == "reset" || command == "rm-all" || command == "destroy-all")
    {
        clear();
    }
    else if (command == "help" || command == "h")
    {
        printHelp();
    }
    else
    {
        std::cerr << "\033[31m❌ Unknown command: " << command << "\033[0m" << std::endl;
    }
}

// Saving and Loading
void TaskManager::save()
{
    jsonFile.saveToFile(tasks);
}

void TaskManager::load()
{
    tasks = jsonFile.loadFromFile();
}

void TaskManager::clear()
{
    if (promptForConfirmation("⚠️ WARNING: This will delete all tasks and cannot be undone."))
    {
        tasks.clear();
        save();
        std::cout << "\033[32m✅ Success: All tasks cleared.\033[0m" << std::endl;
    }
    else
    {
        std::cout << "\033[36mℹ️ Action canceled: No tasks were cleared.\033[0m" << std::endl;
    }
}
