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

    void displayInfo() const override {}
    double getProgress() const override { return 0.0; }

    static string priorityToString(Priority p) {
        if (p == Priority::LOW)  return "Low";
        if (p == Priority::HIGH) return "High";
        return "Medium";
    }
};
