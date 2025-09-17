# Minishell

A UNIX shell implementation written in C.
The goal of this project is to create a functional shell that mimics the behavior of bash for a subset of its features, while deepening understanding of processes, file descriptors, signals, parsing, and system calls.

---

## Features
### Core Shell Behavior
+ Displays a prompt while waiting for commands.
+ Maintains a working command history.
+ Executes commands by searching $PATH, or using absolute/relative paths.
+ Manages one global variable only (to store a signal number).

### Parsing & Input Handling
+ Supports single quotes ' (preventing all expansions).
+ Supports double quotes " (preventing expansions except $).
+ Ignores invalid/unclosed quotes and special characters not required by subject (e.g. ;, \).

### Redirections
+ < Redirect input.
+ \> Redirect output (overwrite).
+ \>> Redirect output (append).
+ << Here-document (reads input until a delimiter line is found, no history update required).

### Pipes
+ Full support for pipes (|) connecting multiple commands.
+ Correctly handles stdin/stdout redirection in pipelines.

### Environment Variables
+ Expands environment variables ($VAR).
+ Supports $? for last exit status.

### Signal Handling
+ ctrl-C → interrupts current command, displays new prompt on new line.
+ ctrl-D → exits shell.
+ ctrl-\ → does nothing (ignored).

### Built-in Commands
+ echo [-n] → print text with optional no-newline.
+ cd [path] → change working directory (absolute or relative).
+ pwd → print working directory.
+ export → set environment variables.
+ unset → unset environment variables.
+ env → display current environment variables.
+ exit → exit shell.

---

## Project Structure

```
├── includes
│   └── minishell.h
├── libft/
├── Makefile
├── README.md
├── valgrind.supp
└── srcs/
    ├── builtins/               #Deals with all the builtin commands
    │   ├── built_in.c
    │   ├── built_in_utils.c
    │   ├── cd.c
    │   ├── check_choose.c
    │   ├── echo.c
    │   ├── env.c
    │   ├── export2.c
    │   ├── export.c
    │   ├── ft_exit.c
    │   ├── pwd.c
    │   ├── set_unset.c
    │   └── unset.c
    ├── execution/              #Deals with all the execution
    │   ├── begin_exec.c
    │   ├── execute.c
    │   ├── execute_utils.c
    │   ├── full_path2.c
    │   ├── full_path.c
    │   ├── heredoc_expand.c
    │   ├── heredocs.c
    │   ├── init.c
    │   ├── pipe2.c
    │   ├── pipe.c
    │   ├── pipe_utils.c
    │   ├── redirection2.c
    │   ├── redirection.c
    │   ├── redirection_utils.c
    │   ├── signals.c
    │   └── utils/
    ├── main.c
    └── parsing/                #Deals with all the parsing (Made completely by npagnon)
        ├── env_var.c
        ├── env_var_utils.c
        ├── free_parse.c
        ├── get_cmd.c
        ├── get_cmd_utils.c
        ├── get_tokens.c
        ├── handle_dollar.c
        ├── handle_tilde.c
        ├── parse_init.c
        └── parse_utils.c
```

---

## Installation

### Clone and build
```bash
git clone https://github.com/ngaurama/Minishell.git
cd Minishell
make
```
This will generate the executable:

`minishell`

### Usage
`./minishell` and then just like a normal shell

### Learning Outcomes
Working on minishell deepened my understanding of:
+ System Programming Fundamentals
+ Using low-level UNIX syscalls (fork, execve, pipe, dup2, waitpid) to manage processes.
+ Manipulating file descriptors for redirections and piping.
+ Process & Job Control
+ Understanding parent/child relationships.
+ Synchronizing multiple processes in a pipeline.
+ Signal Handling
+ Memory Management
+ Shell Architecture
+ Designing a modular system: parsing → AST/execution → cleanup.
+ And most importantly teamwork
    + Coordinating parsing and execution responsibilities.
    + Writing code that integrates cleanly with someone else’s work
---
## Author
+ Nitai Gauramani
  - 42 Paris – Common Core project <br>

![Nitai's GitHub stats](https://github-readme-stats.vercel.app/api?username=ngaurama&show_icons=true&theme=transparent)
