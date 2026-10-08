---

# 🐚 MiniShell

A fully modular Unix shell implementation in C, featuring a complete parsing pipeline, AST-based execution, environment expansion, and process management.

---

## 📌 Overview

MiniShell is a custom shell that replicates core behavior of Bash, including:

* Command execution
* Pipes and redirections
* Environment variable expansion
* Built-in commands
* Signal handling

---

## 🧠 Architecture

```text
input → lexer → tokenizer → parser → AST → expansion → execution
```

### 🔄 Execution Flow

1. **Input**

   * `get_shell_line.c` reads user input using readline

2. **Lexing & Tokenization**

   * `lex.c`, `tokenize.c`
   * Splits input into meaningful tokens (words, pipes, redirections)

3. **Parsing**

   * `parse_line.c`, `parse_validation.c`
   * Builds an **Abstract Syntax Tree (AST)**:

     * `ast.c`, `ast_cmd.c`, `ast_redir.c`

4. **Expansion**

   * Located in `src/expansions/`
   * Handles:

     * `$VAR`
     * `$?`
     * Complex expansion edge cases

5. **Execution**

   * `src/exec/`
   * Traverses AST and executes:

     * pipelines
     * redirections
     * builtins vs external commands

---

## 🗂️ Project Structure

```text
MiniShell/
├── include/                 # Header files
│   ├── minishell.h
│   ├── minishell_parser.h
│   ├── minishell_signal.h
│   └── minishell_errors.h
│
├── src/
│   ├── minishell_main.c     # Entry point
│
│   ├── init/                # Initialization & input
│   │   ├── minishell_initialize.c
│   │   └── get_shell_line.c
│
│   ├── parser/              # Lexing, parsing, AST
│   │   ├── lex.c
│   │   ├── tokenize.c
│   │   ├── parse_line.c
│   │   ├── ast.c
│   │   ├── ast_cmd.c
│   │   └── ast_redir.c
│
│   ├── expansions/          # Variable expansion system
│   │   ├── line_var_expand.c
│   │   ├── advanced_expansions.c
│   │   └── helpers...
│
│   ├── exec/                # Execution engine
│   │   ├── execute.c
│   │   ├── execute_cmd.c
│   │   ├── execute_cmd_redir.c
│   │   ├── heredoc.c
│   │   └── safe_fork.c / safe_pipe.c
│
│   ├── builtins/            # Built-in commands
│   │   ├── builtins_dispatcher.c
│   │   ├── builtins_echo.c
│   │   ├── builtins_cd_pwd_env.c
│   │   ├── builtins_export.c
│   │   ├── builtins_unset.c
│   │   └── builtins_exit.c
│
│   ├── signals/             # Signal handling
│   │   ├── signals.c
│   │   ├── signals_interactive.c
│   │   └── signals_execution.c
│
│   └── utils/               # Utilities & helpers
│       ├── utils_string.c
│       ├── utils_malloc.c
│       ├── utils_error.c
│       ├── utils_fd.c
│       ├── utils_path.c
│       └── env helpers...
│
├── libft/                   # Custom standard library
├── Makefile
└── rl.supp                  # Readline suppression file (valgrind)
```

---

## ✨ Features

### ✅ Core Shell

* Execute commands via `$PATH`
* Support for absolute and relative paths
* Built-in command dispatcher

### 🔗 Pipes & Redirections

* Pipes: `|`
* Input: `<`
* Output: `>`
* Append: `>>`
* Heredoc: `<<`

### 🔤 Expansions

* `$VAR`
* `$?`
* Advanced expansion handling (edge cases covered)

### 🛠 Built-in Commands

* `echo`
* `cd`
* `pwd`
* `export`
* `unset`
* `env`
* `exit`

### ⚡ Signal Handling

* Interactive signals (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)
* Separate handling for:

  * interactive mode
  * execution
  * heredoc

---

## 🔥 Notable Design Choices

### 🌳 AST-Based Execution

Unlike simpler command-table approaches, this shell builds an **Abstract Syntax Tree**, allowing:

* cleaner execution logic
* easier extension (e.g. `&&`, `||`)
* better handling of nested structures

### 🧩 Modular Expansion System

Expansion logic is split across multiple files:

* basic expansion
* advanced cases
* helper layers

This avoids monolithic parsing code and improves maintainability.

### 🛡 Safe System Wrappers

Custom wrappers:

* `safe_fork`
* `safe_pipe`

Ensure:

* proper error handling
* cleaner execution flow

---

## ⚙️ Compilation

```bash
make
```

Other targets:

```bash
make clean
make fclean
make re
```

---

## 🚀 Usage

```bash
./minishell
```

Example:

```bash
minishell$ echo Hello
minishell$ ls -l | grep minishell
minishell$ export NAME=MiniShell
minishell$ echo $NAME
```

---

### Run with Valgrind

```bash
valgrind --suppressions=rl.supp --leak-check=full ./minishell
```


## ⚠️ Limitations

* No job control (`fg`, `bg`)
* No wildcard expansion (`*`)
* No logical operators (`&&`, `||`) *(AST allows future support)*

## 🧠 What This Project Demonstrates

* Process control (`fork`, `execve`, `waitpid`)
* File descriptor manipulation (`dup2`, pipes)
* Parsing and AST construction
* Memory management in C
* Shell behavior replication


## 👤 Author

* **orhan / NameNotHere**
  GitHub: [https://github.com/NameNotHere](https://github.com/NameNotHere)
    -- plus thomas
