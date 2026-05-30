#pragma once
#include "Trackable.h"
#include "Task.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

using namespace std;

class Project : public Trackable {
    vector<unique_ptr<Task>> tasks;
    time_t deadline;

public:
    Project() : Trackable(""), deadline(0) {}

    Project(const string& title, const string& description, time_t deadline)
        : Trackable(title, description), deadline(deadline) {
        if (title.empty()) throw invalid_argument("Empty project title");
    }

    time_t getDeadline() const   { return deadline; }
    void   setDeadline(time_t d) { deadline = d; }

    void addTask(Task* task) {
        if (!task) throw invalid_argument("Null task");
        tasks.emplace_back(task);
    }

    bool removeTask(int taskId) {
        for (auto it = tasks.begin(); it != tasks.end(); ++it)
            if ((*it)->getId() == taskId) { tasks.erase(it); return true; }
        return false;
    }

    Task* findTask(int taskId) const {
        for (const auto& t : tasks)
            if (t->getId() == taskId) return t.get();
        return nullptr;
    }

    vector<Task*> getTasks() const {
        vector<Task*> result;
        for (const auto& t : tasks) result.push_back(t.get());
        return result;
    }

    double getProgress() const override {
        if (tasks.empty()) return 0.0;
        double total = 0;
        for (const auto& t : tasks) total += t->getProgress();
        return total / tasks.size();
    }

    void displayInfo() const override {}
};
