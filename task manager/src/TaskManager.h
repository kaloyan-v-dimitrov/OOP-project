#pragma once
#include "Project.h"
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

using namespace std;

class TaskManager {
    vector<unique_ptr<Project>> projects;

    Project* findProject(int id) const {
        for (const auto& p : projects)
            if (p->getId() == id) return p.get();
        return nullptr;
    }

public:
    TaskManager() {}

    explicit TaskManager(vector<unique_ptr<Project>> projects)
        : projects(move(projects)) {}


    Project* createProject(const string& title,
                           const string& description = "",
                           time_t deadline = 0) {
        auto* p = new Project(title, description, deadline);
        projects.emplace_back(p);
        return p;
    }

    bool deleteProject(int id) {
        for (auto it = projects.begin(); it != projects.end(); ++it)
            if ((*it)->getId() == id) { projects.erase(it); return true; }
        return false;
    }

    bool editProject(int id, const string& title,
                     const string& desc, time_t deadline) {
        auto* p = findProject(id);
        if (!p) return false;
        p->setTitle(title);
        p->setDescription(desc);
        p->setDeadline(deadline);
        return true;
    }

    vector<Project*> getProjects() const {
        vector<Project*> result;
        for (const auto& p : projects) result.push_back(p.get());
        return result;
    }

    Task* addTask(int projectId, const string& title,
                  const string& desc = "",
                  Priority priority = Priority::MEDIUM,
                  time_t deadline = 0) {
        auto* proj = findProject(projectId);
        if (!proj) return nullptr;
        auto* t = new Task(title, desc, priority, deadline);
        proj->addTask(t);
        return t;
    }

    bool removeTask(int projectId, int taskId) {
        auto* proj = findProject(projectId);
        return proj && proj->removeTask(taskId);
    }

    bool editTask(int projectId, int taskId,
                  const string& title, const string& desc,
                  Priority priority, time_t deadline) {
        auto* proj = findProject(projectId);
        if (!proj) return false;
        auto* task = proj->findTask(taskId);
        if (!task) return false;
        task->setTitle(title);
        task->setDescription(desc);
        task->setPriority(priority);
        task->setDeadline(deadline);
        return true;
    }


    bool addSubtask(int projectId, int taskId, const string& title) {
        auto* proj = findProject(projectId);
        if (!proj) return false;
        auto* task = proj->findTask(taskId);
        if (!task) return false;
        task->addSubtask(Subtask(title));
        return true;
    }

    bool toggleSubtask(int projectId, int taskId, int subtaskId) {
        auto* proj = findProject(projectId);
        if (!proj) return false;
        auto* task = proj->findTask(taskId);
        return task && task->toggleSubtask(subtaskId);
    }

    const vector<unique_ptr<Project>>& getAllProjects() const { return projects; }
};

TaskManager loadFromFile(const string& filePath);
void        saveToFile(const string& filePath, const TaskManager& manager);
