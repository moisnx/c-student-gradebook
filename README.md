# 📚 C Student Gradebook

A console-based student grade management system built during my first 10 days learning C.

## Features

- Add and manage student records
- Track multiple grades per student (up to 10)
- Calculate grade averages and statistics
- Persistent file storage
- Dynamic memory allocation with resizing arrays

## Usage

```bash
gcc -o gradebook gradebook.c
./gradebook
```

## Sample Output

```
---------------------------------------------------------------
Student Gradebook Program
---------------------------------------------------------------
[1] -> List All Students
[2] -> Add new student
[3] -> Add grade
[4] -> Calculate average
[5] -> Remove student
[0] -> Save & Exit
---------------------------------------------------------------
```

```
| ID   | NAME                 | Grades                    | Average Score |
|------|----------------------|---------------------------|---------------|
| 2    | Zoe Martinez         | 95 87 93 89 96 (+5 more)  | 90.9          |
| 3    | Caleb Thompson       | 72 68 75 80 77 (+1 more)  | 74.2          |
```

## What I Learned

- Dynamic memory management (`malloc`, `realloc`)
- File I/O operations and data persistence
- String processing and input validation
- Structured programming with proper function organization

## Technical Notes

Built using fundamental C concepts including structs, pointers, dynamic arrays, and file handling. Data is stored in a pipe-delimited format for simplicity.

---

_Learning project - January 2025_
