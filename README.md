# EC535 – Embedded Systems Repository

**Owner:** [YashP007](https://github.com/YashP007)  
**Course:** EC535 – Embedded Systems (Boston University)

---

## 📖 Overview
This repository contains all homework (HW) and lab assignments for EC535.  
Each assignment is worked on in its own branch and subfolder:

```

EC535/
├── HW1/
├── HW2/
├── Lab1/
├── Lab2/
└── ...

````

### Workflow
1. Create a new branch for each HW/Lab.
2. Work on the assignment **only** in its dedicated folder.
3. Merge the branch back into `main` after completion.

This structure helps keep assignments isolated while practicing proper Git workflows.

---

## ⚡ Quick Git Commands

### Clone & Set Up
```bash
# First time cloning repo
git clone https://github.com/YashP007/EC535.git
cd EC535

# Configure user identity (if not set)
git config --global user.name "Yash Patel"
git config --global user.email "pately@bu.edu"
````

---

### Pull Latest Changes

```bash
# From main branch
git checkout main
git pull origin main
```

---

### Create a Branch

```bash
# Create and switch to new branch for HW1
git checkout -b HW1

# Switch between branches
git checkout main
git checkout HW1
```

---

### Stage, Commit, and Push

```bash
# Stage all modified files
git add .

# Commit with a clear message
git commit -m "Implemented part 1 of HW1"

# Push branch to GitHub
git push origin HW1
```

---

### Merge Back Into Main

```bash
# Switch to main
git checkout main

# Merge changes from HW1
git merge HW1

# Push updated main branch
git push origin main
```

---

## 🖥️ Compiling & Running C Programs

This course uses C, typically compiled with `gcc`.

### Basic Compilation

```bash
# Compile single file
gcc main.c -o main

# Run the program
./main
```

### With Debugging Flags

```bash
gcc -Wall -Werror -g main.c -o main
gdb ./main
```

### Using a Makefile

If a `Makefile` is present:

```bash
make        # Build project
make clean  # Remove compiled files
```

Reference: [GNU Make Manual](http://www.gnu.org/software/make/manual/make.html)

---

## 🌐 Environment Notes

You'll be working in **three environments**:

1. **Lab Computer (Physical)** – Local Linux desktop in lab.
2. **SSH into Lab Computer** – Remote work over terminal.
3. **WSL on Personal Machine** – Windows Subsystem for Linux.

> Tip: Always check which environment you're in using:
>
> ```bash
> uname -a
> hostname
> ```

Keep Git credentials consistent between environments to avoid conflicts.

---

## 🔑 SSH Setup

If you need to generate SSH keys (e.g., for GitHub or lab machines):

```bash
ssh-keygen -t ed25519 -C "pately@bu.edu"
# Save at: /home/<user>/.ssh/id_ed25519

# Start ssh-agent and add key
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
```

View your public key:

```bash
cat ~/.ssh/id_ed25519.pub
```

---

## 🗂 Class Content Roadmap

| Week  | Topics                                | Labs / HW                  |
| ----- | ------------------------------------- | -------------------------- |
| 1     | Intro + Overview                      |                            |
| 2     | HW/SW Nature, Embedded Cores          | HW1                        |
| 3     | Linux Tutorial                        | Lab1 (QEMU, BusyBox)       |
| 4     | ARM Architecture                      |                            |
| 5     | RTOS Intro + Scheduling               | Lab2 (Device Drivers)      |
| 6-8   | RTOS, IPC, Synchronization            | Lab3 (Async Notifications) |
| 9-11  | Performance Metrics, Cost, Energy     | HW2                        |
| 12-15 | Device Drivers on Hardware, Debugging | Lab4                       |
| 16-19 | Communication, Interrupts, Qt         | HW3, Lab5                  |
| 20-24 | Security Threat Models, TEE, Crypto   | HW4                        |
| 25-28 | Project Demos + Final Review          |                            |

---

## 📝 Scratch Notes & Links

Use this section to keep useful reminders:

* **Makefile Guide:** [GNU Make Manual](http://www.gnu.org/software/make/manual/make.html)
* Common Git merge conflict commands:

  ```bash
  git status
  git diff
  git merge --abort
  ```
* Check process using a port:

  ```bash
  lsof -i :<PORT>
  ```
* QEMU Basics:

  * Boot with rootfs: `qemu-system-arm -M versatilepb -kernel zImage -initrd rootfs.img`

---

## 🧩 To-Do

* [ ] Create initial branch for HW1.
* [ ] Set up `.gitignore` for compiled binaries.
* [ ] Document QEMU usage commands.
* [ ] Add notes for kernel module debugging.

---

## License

This repository is for educational use only.

```

This README is structured so you can **easily add sections** later — for example, dedicated pages for QEMU, kernel modules, or debugging workflows. The scratch notes section at the bottom is a living space for quick commands and links you discover throughout the semester.
```
