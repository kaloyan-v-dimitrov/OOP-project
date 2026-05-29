#pragma once
#include "Trackable.h"
#include "Subtask.h"
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

enum class Priority { LOW, MEDIUM, HIGH };

class Task : public Trackable {
    Priority        priority;
    time_t          deadline;
    vector<Subtask> subtasks;

public:
    Task() : Trackable(""), priority(Priority::MEDIUM), deadline(0) {}

    Task(const string& title, const string& description,
         Priority priority, time_t deadline)
        : Trackable(title, description), priority(priority), deadline(deadline) {
        if (title.empty()) throw invalid_argument("Empty task title");
    }

    Priority               getPriority()  const { return priority; }
    time_t                 getDeadline()  const { return deadline; }
    const vector<Subtask>& getSubtasks()  const { return subtasks; }

    void setPriority(Priority p) { priority = p; }
    void setDeadline(time_t d)   { deadline = d; }

    void addSubtask(const Subtask& s) { subtasks.push_back(s); }

    bool toggleSubtask(int subtaskId) {
        for (auto& s : subtasks)
            if (s.getId() == subtaskId) { s.toggle(); return true; }
        return false;
    }

    bool isOverdue() const {
        if (deadline == 0) return false;
        return time(nullptr) > deadline;
    }

    double getProgress() const override {
        if (subtasks.empty()) return 0.0;
        int done = 0;
        for (const auto& s : subtasks) if (s.getIsDone()) done++;
        return (double)done / subtasks.size() * 100.0;
    }

    void displayInfo() const override {
        cout << "  [" << id << "] " << title
             << " | " << priorityToString(priority)
             << " | Deadline: " << formatDate(deadline)
             << (isOverdue() ? " *** OVERDUE ***" : "") << "\n";
        if (!description.empty())
            cout << "       " << description << "\n";
        if (!subtasks.empty()) {
            cout << "       Subtasks (" << (int)getProgress() << "% complete):\n";
            for (const auto& s : subtasks) s.displayInfo();
        }
    }

    static string priorityToString(Priority p) {
        if (p == Priority::LOW)  return "Low";
        if (p == Priority::HIGH) return "High";
        return "Medium";
    }
};
