# SWYNEX-Application-Logic-in-C
# SWYNEX Task Scheduler (C++17)

A menu-driven task scheduler built with C++ Standard Library containers and algorithms. This project extends the earlier STL task scheduler into a more usable console application.

## Features

- Add tasks with a required title, optional description/deadline, and priority.
- View all tasks, pending tasks, or completed tasks.
- Mark a task as completed and delete tasks by ID.
- Search task titles and sort tasks by priority.
- Display a task summary.
- Automatically save tasks to `tasks.txt` and reload them on the next run.
- Validate menu choices, task IDs, priority values, and required titles.
- Handle missing/unavailable data files and report file read/write problems.

## Data-structure and STL choices

- `vector<Task>` stores the task collection. It is straightforward for a small console application, supports iteration, and works efficiently with the STL algorithms used here.
- `find_if` locates a task by its ID.
- `remove_if` with `erase` deletes a task.
- `sort` orders tasks by priority, with ID as a tie-breaker.
- `count_if` calculates the number of completed tasks.
- `quoted` from `<iomanip>` safely stores text fields containing spaces in the local data file.

## Requirements

- A C++ compiler with C++17 support (GCC, Clang, or MSVC).

## Build and run

### Windows (MinGW/GCC)

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o task_scheduler.exe
task_scheduler.exe
```

### Linux / macOS

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o task_scheduler
./task_scheduler
```

### CMake

```bash
cmake -S . -B build
cmake --build build
```

Run the generated executable from the project directory so `tasks.txt` is created there.

## Menu

```text
1. Add task
2. View all tasks
3. Mark task as completed
4. Delete task
5. Search tasks by title
6. Sort tasks by priority
7. View pending tasks
8. View completed tasks
9. Task summary
0. Save and exit
```

Priority `1` means highest priority and `5` means lowest priority.

## Error handling notes

- Invalid menu, ID, and priority input is rejected and the user is prompted again.
- Empty task titles and empty search terms are rejected.
- Unknown task IDs are reported without changing the task list.
- The application reports file read/write problems instead of silently claiming that saving succeeded.
- A missing `tasks.txt` file is treated as a first run.

## Repository

Suggested GitHub repository name: `SWYNEX-Task-Scheduler`

After creating the repository on GitHub, push these project files and submit your repository URL to SWYNEX.
