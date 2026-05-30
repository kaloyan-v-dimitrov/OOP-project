#include "TaskManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

TaskManager loadFromFile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) throw runtime_error("Cannot open file: " + filePath);
    return TaskManager();
}

void saveToFile(const string& filePath, const TaskManager& manager) {
    ofstream file(filePath);
    if (!file.is_open()) throw runtime_error("Cannot open file: " + filePath);
}
