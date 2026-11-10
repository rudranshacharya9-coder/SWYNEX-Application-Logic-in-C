#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Task {
    int id{};
    string title;
    string description;
    int priority{};       // 1 = highest, 5 = lowest
    string deadline;      // Free-form date, e.g. 2026-11-20
    bool completed{};
};

class TaskScheduler {
private:
    vector<Task> tasks;
    int nextId = 1;
    const string dataFile = "tasks.txt";

    static string readLine(const string& prompt) {
        cout << prompt;
        string value;
        getline(cin, value);
        return value;
    }

    static int readInt(const string& prompt, int minimum, int maximum) {
        while (true) {
            string input = readLine(prompt);
            stringstream ss(input);
            int value;
            char extra;
            if ((ss >> value) && !(ss >> extra) &&
                value >= minimum && value <= maximum) {
                return value;
            }
            cout << "Invalid input. Enter a number from "
                 << minimum << " to " << maximum << ".\n";
        }
    }

    Task* findTask(int id) {
        auto it = find_if(tasks.begin(), tasks.end(),
                          [id](const Task& task) { return task.id == id; });
        return it == tasks.end() ? nullptr : &(*it);
    }

    static void printTask(const Task& task) {
        cout << "\nID: " << task.id
             << "\nTitle: " << task.title
             << "\nDescription: " << (task.description.empty() ? "(none)" : task.description)
             << "\nPriority: " << task.priority << " (1 = highest)"
             << "\nDeadline: " << (task.deadline.empty() ? "(not set)" : task.deadline)
             << "\nStatus: " << (task.completed ? "Completed" : "Pending")
             << "\n-----------------------------\n";
    }

    void saveToFile() const {
        ofstream out(dataFile);
        if (!out) {
            cerr << "Warning: Could not save tasks to " << dataFile << ".\n";
            return;
        }
        for (const auto& task : tasks) {
            out << task.id << ' '
                << task.priority << ' '
                << task.completed << ' '
                << quoted(task.title) << ' '
                << quoted(task.description) << ' '
                << quoted(task.deadline) << '\n';
        }
        if (!out) {
            cerr << "Warning: An error occurred while writing task data.\n";
        }
    }

    void loadFromFile() {
        ifstream in(dataFile);
        if (!in) return; // First run: no file exists yet.

        Task task;
        while (in >> task.id >> task.priority >> task.completed
                  >> quoted(task.title) >> quoted(task.description)
                  >> quoted(task.deadline)) {
            if (task.id > 0 && task.priority >= 1 && task.priority <= 5) {
                tasks.push_back(task);
                nextId = max(nextId, task.id + 1);
            }
        }
        if (!in.eof()) {
            cerr << "Warning: Some saved data could not be read. "
                 << "Valid records were loaded.\n";
        }
    }

    void addTask() {
        Task task;
        task.id = nextId++;
        do {
            task.title = readLine("Task title (required): ");
            if (task.title.find_first_not_of(" \t") == string::npos)
                cout << "Title cannot be empty. Please try again.\n";
        } while (task.title.find_first_not_of(" \t") == string::npos);

        task.description = readLine("Description (optional): ");
        task.priority = readInt("Priority (1 = highest, 5 = lowest): ", 1, 5);
        task.deadline = readLine("Deadline (optional, e.g. 2026-11-20): ");
        task.completed = false;

        tasks.push_back(task);
        saveToFile();
        cout << "Task added successfully. ID: " << task.id << "\n";
    }

    void showTasks() const {
        if (tasks.empty()) {
            cout << "No tasks found.\n";
            return;
        }
        for (const auto& task : tasks) printTask(task);
    }

    void markCompleted() {
        if (tasks.empty()) {
            cout << "There are no tasks to update.\n";
            return;
        }
        int id = readInt("Enter task ID: ", 1, numeric_limits<int>::max());
        Task* task = findTask(id);
        if (!task) {
            cout << "No task exists with ID " << id << ".\n";
            return;
        }
        if (task->completed) {
            cout << "That task is already completed.\n";
            return;
        }
        task->completed = true;
        saveToFile();
        cout << "Task marked as completed.\n";
    }

    void deleteTask() {
        if (tasks.empty()) {
            cout << "There are no tasks to delete.\n";
            return;
        }
        int id = readInt("Enter task ID to delete: ", 1, numeric_limits<int>::max());
        auto oldSize = tasks.size();
        tasks.erase(remove_if(tasks.begin(), tasks.end(),
                              [id](const Task& task) { return task.id == id; }),
                    tasks.end());
        if (tasks.size() == oldSize) {
            cout << "No task exists with ID " << id << ".\n";
        } else {
            saveToFile();
            cout << "Task deleted successfully.\n";
        }
    }

    void searchTasks() const {
        string keyword = readLine("Enter a word to search in task titles: ");
        if (keyword.empty()) {
            cout << "Search text cannot be empty.\n";
            return;
        }
        bool found = false;
        for (const auto& task : tasks) {
            if (task.title.find(keyword) != string::npos) {
                printTask(task);
                found = true;
            }
        }
        if (!found) cout << "No matching tasks found.\n";
    }

    void sortByPriority() {
        sort(tasks.begin(), tasks.end(),
             [](const Task& a, const Task& b) {
                 if (a.priority != b.priority) return a.priority < b.priority;
                 return a.id < b.id;
             });
        saveToFile();
        cout << "Tasks sorted by priority (highest first).\n";
        showTasks();
    }

    void showByStatus(bool completed) const {
        bool found = false;
        for (const auto& task : tasks) {
            if (task.completed == completed) {
                printTask(task);
                found = true;
            }
        }
        if (!found) cout << "No " << (completed ? "completed" : "pending")
                         << " tasks found.\n";
    }

    void showSummary() const {
        size_t completed = count_if(tasks.begin(), tasks.end(),
                                    [](const Task& t) { return t.completed; });
        cout << "\nTotal tasks: " << tasks.size()
             << "\nPending: " << tasks.size() - completed
             << "\nCompleted: " << completed << "\n";
    }

public:
    TaskScheduler() { loadFromFile(); }

    void run() {
        cout << "=================================\n"
             << "       SWYNEX TASK SCHEDULER\n"
             << "=================================\n";
        while (true) {
            cout << "\n1. Add task\n"
                 << "2. View all tasks\n"
                 << "3. Mark task as completed\n"
                 << "4. Delete task\n"
                 << "5. Search tasks by title\n"
                 << "6. Sort tasks by priority\n"
                 << "7. View pending tasks\n"
                 << "8. View completed tasks\n"
                 << "9. Task summary\n"
                 << "0. Save and exit\n";

            int choice = readInt("Choose an option: ", 0, 9);
            switch (choice) {
                case 1: addTask(); break;
                case 2: showTasks(); break;
                case 3: markCompleted(); break;
                case 4: deleteTask(); break;
                case 5: searchTasks(); break;
                case 6: sortByPriority(); break;
                case 7: showByStatus(false); break;
                case 8: showByStatus(true); break;
                case 9: showSummary(); break;
                case 0:
                    saveToFile();
                    cout << "Tasks saved. Goodbye!\n";
                    return;
            }
        }
    }
};

int main() {
    try {
        TaskScheduler app;
        app.run();
    } catch (const exception& ex) {
        cerr << "Unexpected error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
