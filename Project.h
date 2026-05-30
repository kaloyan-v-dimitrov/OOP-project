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

    double getProgress() const override { return 0.0; }
    void   displayInfo() const override {}
};
