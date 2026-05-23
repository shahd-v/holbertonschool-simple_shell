# Simple Shell (hsh)

A minimal UNIX command interpreter written in C, recreating a useful subset
of `/bin/sh` behavior. Built as a team project for the Holberton School
Low-Level Programming curriculum.

---

## Table of Contents

- [Description](#description)
- [Background](#background)
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [How It Works](#how-it-works)
- [File Structure](#file-structure)
- [Examples](#examples)
- [Limitations](#limitations)
- [Authors](#authors)

---

## Description

`hsh` (Holberton shell) is a simple UNIX command-line interpreter. It reads
commands from standard input — either interactively from a terminal or
non-interactively from a pipe or file — splits each command into arguments,
resolves the program against the `PATH` environment variable, and executes
it in a forked child process.

The shell mimics `/bin/sh`'s error format exactly, with the program-name
prefix taken from `argv[0]` so the same binary produces an appropriate
message regardless of how it was invoked.

## Background

The first UNIX shell was written by **Ken Thompson** at Bell Labs in 1971,
as part of the original UNIX operating system that he and Dennis Ritchie
designed. Thompson also created the **B programming language**, the direct
predecessor of C. This project pays tribute to that lineage by building a
small interpreter from scratch using only low-level system calls.

## Installation

Clone the repository and compile with the required flags:

```bash
git clone https://github.com/shahd-v/holbertonschool-simple_shell.git
cd holbertonschool-simple_shell
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

The resulting executable is `hsh`.

## Usage

### Interactive mode

Run `./hsh` from a terminal:

```
$ ./hsh
WriteSomeThing;>$ /bin/ls
README.md  hsh  path.c  shell.c  shell.h
WriteSomeThing;>$ ls -l /tmp
total 0
WriteSomeThing;>$ ^D
$
```

Press **Ctrl-D** (EOF) to exit.

### Non-interactive mode

Pipe commands into the shell or feed it a file:

```
$ echo "/bin/ls" | ./hsh
README.md  hsh  path.c  shell.c  shell.h

$ cat commands.txt | ./hsh
```

## Features

- Interactive prompt when stdin is a terminal (TTY)
- Non-interactive mode for pipes and redirected input
- Argument tokenization on whitespace
- Full `PATH` resolution — `ls` works, not just `/bin/ls`
- Explicit-path handling (`./script`, `/bin/echo`) without PATH search
- **`fork` is never called when the command doesn't exist** (the task 4 rule)
- `sh`-compatible error: `program: line: command: not found`
- Program name in errors matches `argv[0]` exactly
- Clean EOF handling
- Zero memory leaks (verified with valgrind)
- Betty-style code, ≤5 functions per file

## How It Works

```
   ┌────────────────────────────────────────────────────┐
   │                       main                          │
   │  while (1):                                         │
   │    print prompt (if TTY) → read_line → process_line │
   └─────────────────────────┬───────────────────────────┘
                             │
                             ▼
   ┌────────────────────────────────────────────────────┐
   │                  process_line                       │
   │  trim → is_empty? → split_line → find_in_path       │
   │                                       │             │
   │                              ┌────────┴──────────┐  │
   │                          NULL│                   │OK│
   │                              ▼                   ▼  │
   │                  print "not found"         handle_fork
   │                  (no fork)                                    
   └────────────────────────────────────────────────────┘
```

### PATH resolution

When `find_in_path` receives a command name:

1. If the name contains `/`, it's treated as an explicit path and used
   directly (if `access()` says it exists).
2. Otherwise, `PATH` is read via a custom `_getenv` (because `getenv` is
   not on the allowed function list) and split on `:`.
3. Each directory is joined with the command name (`/usr/bin` + `ls` →
   `/usr/bin/ls`) and tested with `access(F_OK)`.
4. The first match is returned as a freshly malloc'd string. If nothing
   matches, NULL is returned.

### Process model

- **`fork()`** creates a child process that is a copy of the shell.
- **`execve()`** in the child replaces its memory image with the target
  program. The parent's memory is untouched.
- **`wait()`** in the parent suspends execution until the child exits,
  so the prompt only reappears after the command finishes.

### Why "no fork if not found"?

`fork()` duplicates the entire process — page tables, file descriptors,
the works. Forking only to immediately discover the command doesn't exist
is wasteful, so we resolve the path **before** forking and bail out early
on failure. This is also what `bash` and `sh` do.

## File Structure

| File         | Role                                                    |
|--------------|---------------------------------------------------------|
| `shell.h`    | Header — includes, prototypes, `environ` declaration    |
| `shell.c`    | `main`, the REPL loop, `process_line`, fork/exec wiring |
| `path.c`     | `_getenv`, `build_path`, `find_in_path` (PATH logic)    |
| `tokenize.c` | To design and cutting the arguments                     |
| `builituns.c`| is_number method and env printer method                 |
| `README.md`  | This file                                               |
| `AUTHORS`    | List of project contributors                            |
| `man_1_simple_shell` | Manual page (groff format)                      |

Read the man page with:

```bash
man ./man_1_simple_shell
```

## Examples

**Command not found** — program name reflects `argv[0]`:

```
$ echo "qwerty" | ./hsh
./hsh: 1: qwerty: not found

$ echo "qwerty" | ./././hsh
./././hsh: 1: qwerty: not found
```

**Commands with arguments**:

```
$ echo "ls -la /tmp" | ./hsh
total 28
drwxrwxrwt  6 root root 4096 May 18 13:44 .
drwxr-xr-x 22 root root 4096 May 18 13:35 ..
...
```

**Multi-line scripts via pipe**:

```
$ printf "ls\n/bin/echo hello world\npwd\n" | ./hsh
README.md  hsh  path.c  shell.c  shell.h
hello world
/home/shahd/projects/holbertonschool-simple_shell
```

## Limitations

This is intentionally a minimal shell. Not (yet) supported:

- Built-ins (`cd`, `exit`, `env`)
- Pipes (`|`) and I/O redirection (`>`, `<`, `>>`)
- Variable expansion (`$VAR`, `$?`)
- Quoting (`"hello world"` is split into two tokens)
- Background jobs (`&`)
- Logical operators (`&&`, `||`, `;`)
- Command history / line editing
- Globbing (`*.c`)

## Allowed Functions

This project may only use these functions (per requirements):
`access`, `execve`, `fork`, `wait`, `waitpid`, `getline`, `strtok`,
`malloc`, `free`, `perror`, `printf`, `fprintf`, `write`, `isatty`,
`exit`, plus anything from `string.h`, and the `environ` global.

## Authors

See [`AUTHORS`](AUTHORS) for the full list.

- **Leen Algraawi** — [github profile]
- **Shahd Shahd** — [shahd-v](https://github.com/shahd-v)

## License

Educational project for Holberton School. No formal license; please
attribute the authors if you reuse any part of this code.
