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

    void displayInfo() const override {
        cout << "=== Project [" << id << "]: " << title
             << " | Deadline: " << formatDate(deadline)
             << " | Progress: " << (int)getProgress() << "% ===\n";
        if (!description.empty()) cout << "    " << description << "\n";
        if (tasks.empty()) cout << "    (no tasks)\n";
        else for (const auto& t : tasks) t->displayInfo();
    }

    friend ostream& operator<<(ostream& out, const Project& p) {
        out << p.id << " " << p.deadline << " " << p.title << "\n";
        if (!p.description.empty()) out << p.description; else out << "-";
        out << "\n";
        for (const auto& t : p.tasks)
            out << *t << "-----------------------\n";
        return out;
    }

    friend istream& operator>>(istream& in, Project& p) {
        int pid;
        time_t dl;
        string title, desc;

        if (!(in >> pid >> dl)) return in;
        in.ignore();
        if (!getline(in, title) || title.empty()) { in.setstate(ios::failbit); return in; }
        if (!getline(in, desc))                   { in.setstate(ios::failbit); return in; }
        if (desc == "-") desc = "";

        p.id          = pid;
        p.deadline    = dl;
        p.title       = title;
        p.description = desc;
        p.tasks.clear();
        return in;
    }
};
