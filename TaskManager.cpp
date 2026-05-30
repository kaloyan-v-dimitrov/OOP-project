#include "TaskManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;



TaskManager loadFromFile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) throw runtime_error("Cannot open file: " + filePath);

    vector<unique_ptr<Project>> projects;
    int maxId = 0;

    Project* curProject = nullptr;
    Task*    curTask    = nullptr;

    string line;
    while (getline(file, line)) {
        if (line.empty() || line == "-----------------------") {
            curTask = nullptr;
            continue;
        }

        istringstream ss(line);
        string type;
        ss >> type;

        if (type == "PROJECT") {
            int pid; time_t dl; string title, desc;
            ss >> pid >> dl;
            ss.ignore();
            getline(ss, title);
            getline(file, desc);
            if (desc == "-") desc = "";

            maxId = max(maxId, pid);
            auto* p = new Project(title, desc, dl);
            projects.emplace_back(p);
            curProject = p;
            curTask    = nullptr;

        } else if (type == "TASK" && curProject) {
            int tid, prio; time_t dl; string title, desc;
            ss >> tid >> prio >> dl;
            ss.ignore();
            getline(ss, title);
            getline(file, desc);
            if (desc == "-") desc = "";

            maxId = max(maxId, tid);
            auto* t = new Task(title, desc, (Priority)prio, dl);
            curProject->addTask(t);
            curTask = t;

        } else if (type == "SUBTASK" && curTask) {
            int sid, done; string title;
            ss >> sid >> done;
            ss.ignore();
            getline(ss, title);

            maxId = max(maxId, sid);
            Subtask s(title);
            if (done) s.toggle();
            curTask->addSubtask(s);
        }
    }

    Trackable::resetNextId(maxId + 1);
    return TaskManager(move(projects));
}

void saveToFile(const string& filePath, const TaskManager& manager) {
    ofstream file(filePath);
    if (!file.is_open()) throw runtime_error("Cannot open file: " + filePath);

    for (const auto& proj : manager.getAllProjects()) {
        file << "PROJECT " << proj->getId() << " " << proj->getDeadline()
             << " " << proj->getTitle() << "\n";
        file << (proj->getDescription().empty() ? "-" : proj->getDescription()) << "\n";

        for (auto* t : proj->getTasks()) {
            file << "TASK " << t->getId() << " " << (int)t->getPriority()
                 << " " << t->getDeadline() << " " << t->getTitle() << "\n";
            file << (t->getDescription().empty() ? "-" : t->getDescription()) << "\n";

            for (const auto& s : t->getSubtasks())
                file << "SUBTASK " << s.getId() << " " << s.getIsDone()
                     << " " << s.getTitle() << "\n";
        }
        file << "-----------------------\n";
    }
}
