#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include "types/tasks/task.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

class jsonManager {
public:
    std::string fileName;
    jsonManager(const char *fileName);
    void create();
    static std::string tasksToJson(const std::vector<Task>& tasks);
    static std::vector<Task> jsonToTasks(const std::string& json);

    // File handling methods
    void saveToFile(const std::vector<Task>& tasks);
    std::vector<Task> loadFromFile();
};

#endif // JSONMANAGER_H
