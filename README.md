# Shared Whiteboard (USP Mini Project)

An **interactive terminal-based whiteboard** developed in **C** using **System V IPC (Shared Memory & Semaphores)**.  
This project demonstrates core Unix System Programming concepts such as **process creation, inter-process communication, synchronization, and signal handling**.

---

## Overview

The project consists of two programs:
- **controller.c** → Creates and manages the shared memory whiteboard, semaphore, and viewer process.
- **user.c** → Allows multiple users to connect and draw/write on the shared board simultaneously.

Each user process communicates with the controller via **shared memory**, and **semaphores** ensure that only one user writes at a time to prevent race conditions.

---

## Features

- Real-time multi-user text and line drawing on a shared whiteboard.
- Process synchronization using **System V semaphores**.
- Signal-based cleanup for safe removal of IPC resources.
- Live terminal-based display refreshed using **ANSI escape sequences**.
- Multi-process architecture using `fork()` and `kill()` system calls.

---

## System Calls Used

| Concept | System Calls |
|----------|---------------|
| Process Creation | `fork()`, `wait()`, `kill()` |
| Shared Memory (IPC) | `ftok()`, `shmget()`, `shmat()`, `shmdt()`, `shmctl()` |
| Semaphores (Sync) | `semget()`, `semctl()`, `semop()` |
| Signals | `signal()`, `SIGINT`, `SIGTERM` |
| Terminal I/O | ANSI Escape Sequences for clear screen |

---

## Concepts Demonstrated

- **Process Management:** Creating and managing multiple processes (`controller`, `viewer`, `user`).
- **Inter-Process Communication:** Shared memory segment for real-time board updates.
- **Synchronization:** Preventing concurrent writes using semaphores.
- **Signal Handling:** Graceful resource cleanup on `Ctrl + C`.
- **Terminal Control:** Refreshing display dynamically with escape codes.

---

## System Requirements

This project uses **System V IPC**, which is supported only on **UNIX/Linux systems**.

> **Note:**  
> The code will **not compile or run on Windows**, because headers like  
> `<sys/ipc.h>`, `<sys/shm.h>`, and `<sys/sem.h>` are Linux-specific.

### Recommended Setup
- **OS:** Ubuntu 20.04 / 22.04 / any Linux distro  
- **Compiler:** GCC  
- **Terminal:** GNOME Terminal / VMware Ubuntu / WSL (Windows Subsystem for Linux)

---

## How to Compile and Run

1. **Clone this repository:**
   ```bash
   git clone https://github.com/<your-username>/shared-whiteboard.git
   cd shared-whiteboard
````

2. **Compile the programs:**

   ```bash
   gcc controller.c -o controller
   gcc user.c -o user
   ```

3. **Run the controller in one terminal:**

   ```bash
   ./controller
   ```

4. **In another terminal, run the user program:**

   ```bash
   ./user
   ```

5. **Use the interactive prompts:**

   ```
   Choose mode (text/line/exit): text
   Enter row: 5
   Enter col: 10
   Enter text: HELLO

   Choose mode (text/line/exit): line
   Enter r1: 2
   Enter c1: 2
   Enter r2: 2
   Enter c2: 15
   Enter char: -
   ```

6. Press **Ctrl + C** in the controller terminal to clean up all resources.

---

## Example Output (ASCII Preview)

```
........................................
........................................
..--------------.......................
..|..........|.........................
..|..........|.........................
..--------------.......................
........................................
..........HELLO.........................
........................................
```

---

## Future Enhancements

* Add “fill” tool for region coloring.
* Add “box” or “circle” drawing shortcuts.
* Save & restore board snapshots to file.

---
