#pragma once
#include "Trackable.h"
#include <iostream>
#include <stdexcept>

using namespace std;

class Subtask : public Trackable {
    bool isDone;

public:
    Subtask() : Trackable(""), isDone(false) {}

    Subtask(const string& title, const string& description = "")
        : Trackable(title, description), isDone(false) {
        if (title.empty()) throw invalid_argument("Empty subtask title");
    }

    bool getIsDone() const { return isDone; }
    void toggle()          { isDone = !isDone; }

    void displayInfo() const override {
        cout << "      [" << (isDone ? "X" : " ") << "] [" << id << "] " << title << "\n";
    }

    double getProgress() const override { return isDone ? 100.0 : 0.0; }

    friend ostream& operator<<(ostream& out, const Subtask& s) {
        out << s.id << " " << s.isDone << " " << s.title;
        return out;
    }

    friend istream& operator>>(istream& in, Subtask& s) {
        int sid, done;
        string t;
        if (!(in >> sid >> done)) return in;
        getline(in, t);
        if (!t.empty() && t[0] == ' ') t = t.substr(1);
        if (t.empty()) { in.setstate(ios::failbit); return in; }
        s.id     = sid;
        s.isDone = (done == 1);
        s.title  = t;
        return in;
    }
};
