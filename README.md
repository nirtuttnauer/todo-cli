
# TODO CLI Application

## 📋 Overview

The TODO CLI application is a command-line tool for managing tasks effectively. It allows users to add, edit, delete, and list tasks, as well as manage their statuses (`todo`, `in-progress`, and `done`). This lightweight application ensures you stay organized and on top of your to-do list.

This project is an idea of the Roadmap.sh project:
https://roadmap.sh/projects/task-tracker

## ✨ Features

- Add tasks with a description.
- Edit tasks by ID and update descriptions.
- Delete tasks individually or clear all tasks.
- List tasks by status or display all tasks.
- Mark tasks as `todo`, `in-progress`, or `done`.
- Manage tasks with an intuitive command-line interface.

## 🛠️ Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/nirtuttnauer/todo-cli.git
   cd todo-cli
   ```

2. **Build the Project**:
   Ensure you have `cmake` and a C++ compiler installed.
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. **Run the Application**:
   ```bash
   ./todo
   ```

4. **Install the Application**:
   MacOS:
   ```bash
    sudo cp ./todo /usr/local/bin/todo
    ```
    Linux:
    ```bash
    sudo cp ./todo /usr/bin/todo
    ```
    Windows:
    ```bash
    todo.exe
    ```
    
5. **Run the Application**:
   ```bash
   todo
   ```

## 📖 Usage

### Commands

| Command                    | Description                                   |
|----------------------------|-----------------------------------------------|
| `add <description>`        | Add a new task with the provided description. |
| `list`                     | List all tasks.                              |
| `list <status>`            | List tasks filtered by `todo`, `in-progress`, or `done`. |
| `edit <id> <description>`  | Edit the description of a task by its ID.     |
| `delete <id>`              | Delete a task by its ID.                     |
| `status <id>`              | Update the status of a task (`todo`, `in-progress`, or `done`). |
| `clear`                    | Clear all tasks.                             |

### Examples

#### Add a Task
```bash
./todo add "Buy groceries"
```

#### Edit a Task
```bash
./todo edit 1 "Buy groceries and cook dinner"
```

#### List Tasks
```bash
./todo list
```

#### Filter Tasks by Status
```bash
./todo list done
```

#### Mark a Task as `in-progress`
```bash
./todo status 1 <<< "2"
```

#### Delete a Task
```bash
./todo delete 1
```

#### Clear All Tasks
```bash
./todo clear <<< "y"
```

## 🧪 Testing

### Run the Smoke-Test Script
The repository includes `test.sh`, a demonstration script that exercises the CLI.
It prints command output but does not assert expected results, so a successful exit
alone is not a full correctness check.

Build the application first. From the repository root, run the script with a
throwaway home directory so it cannot modify your personal tasks:

```bash
repo_root="$PWD"
test_home="$(mktemp -d)"
mkdir -p "$test_home/Documents"
(cd build && HOME="$test_home" bash "$repo_root/test.sh")
rm -rf "$test_home"
```

The script adds, edits, and deletes tasks, then clears all tasks in its test store.
Do not run it against your normal `HOME` unless you intend to erase those tasks.
The application stores tasks under `$HOME/Documents/.todo-cli/`; the `Documents`
directory must already exist.

The script performs the following:
- Adds multiple tasks.
- Edits task descriptions.
- Deletes specific tasks.
- Marks tasks as `in-progress` or `done`.
- Lists tasks filtered by status.
- Clears all tasks.

### Sample Output
The test script provides detailed outputs, indicating success or failure for each operation.

## 🌟 Features to Add
- Support for due dates and priority levels.
- Export tasks to a file (e.g., JSON or CSV).
- Import tasks from a file.
- Add encryption for sensitive data.
- (MacOS Feature) Create an hidden folder in icloud to store tasks.

## 🛡️ License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a new branch for your feature/bugfix.
3. Commit your changes and submit a pull request.

---

Stay organized and boost your productivity with **TODO CLI**! 🚀
