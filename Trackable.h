#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

class Trackable {
protected:
    int    id;
    string title;
    string description;
    time_t createdAt;

    static int nextId;

    Trackable(const string& title, const string& description = "")
        : id(nextId++), title(title), description(description), createdAt(time(nullptr)) {}

public:
    virtual ~Trackable() {}

    int           getId()          const { return id; }
    const string& getTitle()       const { return title; }
    const string& getDescription() const { return description; }
    time_t        getCreatedAt()   const { return createdAt; }

    void setTitle(const string& t)       { title = t; }
    void setDescription(const string& d) { description = d; }

    static void resetNextId(int v) { nextId = v; }

    static string formatDate(time_t t) {
        if (t == 0) return "none";
        tm* tm_info = localtime(&t);
        ostringstream oss;
        oss << put_time(tm_info, "%d.%m.%Y");
        return oss.str();
    }

    virtual void   displayInfo() const = 0;
    virtual double getProgress() const = 0;
};
