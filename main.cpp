#include <iostream>
#include <string>
#include <limits>
#include <ctime>
#include "TaskManager.h"

using namespace std;

static const string SAVE_FILE = "data.txt";

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int readInt(const string& prompt) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v) { clearInput(); return v; }
        clearInput();
        cout << "  Please enter a whole number.\n";
    }
}

static string readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

static time_t parseDate(const string& s) {
    if (s.empty() || s == "-") return 0;
    tm tm{}; int d, m, y;
    if (sscanf(s.c_str(), "%d.%d.%d", &d, &m, &y) != 3) return 0;
    tm.tm_mday = d; tm.tm_mon = m - 1; tm.tm_year = y - 1900; tm.tm_isdst = -1;
    return mktime(&tm);
}

static Priority readPriority() {
    cout << "  Priority (1=Low, 2=Medium, 3=High): ";
    int c; cin >> c; clearInput();
    if (c == 1) return Priority::LOW;
    if (c == 3) return Priority::HIGH;
    return Priority::MEDIUM;
}

void menuProjects(TaskManager& manager) {
    while (true) {
        cout << "\n-- Projects --\n"
                "  1. Show all\n"
                "  2. Create\n"
                "  3. Edit\n"
                "  4. Delete\n"
                "  0. Back\n";
        int c = readInt("Choice: ");
        if (c == 0) break;

        if (c == 1) {
            auto projs = manager.getProjects();
            if (projs.empty()) cout << "(no projects)\n";
            else for (auto* p : projs) p->displayInfo();

        } else if (c == 2) {
            auto title = readLine("Title: ");
            auto desc  = readLine("Description (Enter to skip): ");
            auto ds    = readLine("Deadline (dd.mm.yyyy or -): ");
            auto* p = manager.createProject(title, desc, parseDate(ds));
            cout << "Created with ID=" << p->getId() << "\n";

        } else if (c == 3) {
            int id     = readInt("Project ID: ");
            auto title = readLine("New title: ");
            auto desc  = readLine("New description: ");
            auto ds    = readLine("New deadline (dd.mm.yyyy or -): ");
            cout << (manager.editProject(id, title, desc, parseDate(ds))
                    ? "Updated.\n" : "Project not found.\n");

        } else if (c == 4) {
            int id = readInt("Project ID: ");
            cout << (manager.deleteProject(id) ? "Deleted.\n" : "Project not found.\n");
        }
    }
}

void menuTasks(TaskManager& manager) {
    while (true) {
        cout << "\n-- Tasks --\n"
                "  1. Add task\n"
                "  2. Edit task\n"
                "  3. Delete task\n"
                "  4. Add subtask\n"
                "  5. Toggle subtask\n"
                "  0. Back\n";
        int c = readInt("Choice: ");
        if (c == 0) break;

        if (c == 1) {
            int pid    = readInt("Project ID: ");
            auto title = readLine("Title: ");
            auto desc  = readLine("Description: ");
            Priority pr = readPriority();
            auto ds    = readLine("Deadline (dd.mm.yyyy or -): ");
            auto* t = manager.addTask(pid, title, desc, pr, parseDate(ds));
            if (t) cout << "Task ID=" << t->getId() << "\n";
            else   cout << "Project not found.\n";

        } else if (c == 2) {
            int pid = readInt("Project ID: ");
            int tid = readInt("Task ID: ");
            auto title = readLine("New title: ");
            auto desc  = readLine("New description: ");
            Priority pr = readPriority();
            auto ds    = readLine("New deadline (dd.mm.yyyy or -): ");
            cout << (manager.editTask(pid, tid, title, desc, pr, parseDate(ds))
                    ? "Updated.\n" : "Task not found.\n");

        } else if (c == 3) {
            int pid = readInt("Project ID: ");
            int tid = readInt("Task ID: ");
            cout << (manager.removeTask(pid, tid) ? "Deleted.\n" : "Task not found.\n");

        } else if (c == 4) {
            int pid    = readInt("Project ID: ");
            int tid    = readInt("Task ID: ");
            auto title = readLine("Subtask title: ");
            cout << (manager.addSubtask(pid, tid, title)
                    ? "Added.\n" : "Task not found.\n");

        } else if (c == 5) {
            int pid = readInt("Project ID: ");
            int tid = readInt("Task ID: ");
            int sid = readInt("Subtask ID: ");
            cout << (manager.toggleSubtask(pid, tid, sid)
                    ? "Toggled.\n" : "Subtask not found.\n");
        }
    }
}

int main() {
    TaskManager manager;
    try {
        manager = loadFromFile(SAVE_FILE);
        cout << "Data loaded from " << SAVE_FILE << "\n";
    } catch (...) {
        cout << "No saved data found, starting fresh.\n";
    }

    cout << "=== Task and Project Manager ===\n";

    while (true) {
        cout << "\n== Main Menu ==\n"
                "  1. Projects\n"
                "  2. Tasks\n"
                "  3. Save\n"
                "  0. Exit\n";
        int c = readInt("Choice: ");
        if (c == 0) break;
        if      (c == 1) menuProjects(manager);
        else if (c == 2) menuTasks(manager);
        else if (c == 3) {
            try {
                saveToFile(SAVE_FILE, manager);
                cout << "Saved to " << SAVE_FILE << "\n";
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
    }

    try { saveToFile(SAVE_FILE, manager); } catch (...) {}
    cout << "Goodbye!\n";
    return 0;
}
