# pa5test

A simple C program to interactively read, write, and seek within a file using low-level system calls (`read`, `write`, `lseek`). Designed for practicing file I/O and error handling on Unix-like systems.

## Features

- **Read**: Read a specified number of bytes from the current file offset.
- **Write**: Write user-inputted text to the file at the current offset.
- **Seek**: Move the file offset using custom `offset` and `whence` values (`0` = SEEK_SET, `1` = SEEK_CUR, `2` = SEEK_END).
- Graceful handling of EOF (Ctrl+D) and memory allocation errors.

## Compilation

```bash
gcc -o pa5test pa5test.c
```
## Usage

```bash
./pa5test <filename>
```

