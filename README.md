<p align="center">
  <img src="https://img.shields.io/badge/42-minishell-blue?style=for-the-badge" alt="42 minishell"/>
</p>

<h1 align="center">Minishell</h1>

<p align="center">
  <em>As beautiful as a shell</em>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-00599C?style=flat-square&logo=c&logoColor=white" alt="C"/>
  <img src="https://img.shields.io/badge/Standard-C99-green?style=flat-square" alt="C99"/>
  <img src="https://img.shields.io/badge/Build-Makefile-orange?style=flat-square" alt="Makefile"/>
  <img src="https://img.shields.io/badge/Library-Readline-red?style=flat-square" alt="Readline"/>
</p>

---

## About

**Minishell** is a simplified Unix shell implementation written in C, developed as part of the [42 School](https://42.fr/) curriculum. The goal is to create a functional shell that mimics the behavior of **bash**, handling command execution, piping, redirections, environment variables, and built-in commands — all while managing memory safely and handling signals properly.

> *This project teaches you about processes, file descriptors, and how a shell actually works under the hood.*

## Authors

| Author | GitHub |
|--------|--------|
| **tkafanov** | [@timofeykafanov](https://github.com/timofeykafanov) |
| **sopperma** | [@SilasLovelace](https://github.com/SilasLovelace) |

---

## Features

### Built-in Commands
| Command | Description |
|---------|-------------|
| `echo` | Display text with `-n` flag support |
| `cd` | Change the current directory (relative/absolute paths, `~`, `-`) |
| `pwd` | Print the current working directory |
| `export` | Set environment variables |
| `unset` | Remove environment variables |
| `env` | Display the current environment |
| `exit` | Exit the shell with optional exit status |

### Shell Features
- **Interactive prompt** displaying the current working directory
- **Command history** via readline (up/down arrow navigation)
- **Pipes** (`|`) — chain commands together
- **Redirections**
  - `>` — output redirection
  - `>>` — output append redirection
  - `<` — input redirection
  - `<<` — heredoc (here-document)
- **Environment variable expansion** (`$VAR`, `$?`)
- **Quote handling** — single quotes (`'`) and double quotes (`"`)
- **Signal handling** — `Ctrl+C`, `Ctrl+D`, `Ctrl+\`
- **Exit status** tracking (`$?`)
- **Syntax error detection** with meaningful error messages

---

## Architecture

The shell follows a classic pipeline architecture:

```
Input → Lexer → Expander → Syntax Check → Parser → Executor → Output
```

```
minishell/
├── includes/
│   ├── minishell.h          # Function prototypes
│   ├── structs.h            # Data structure definitions
│   └── macro.h              # Constants and macros
├── srcs/
│   ├── main.c               # Entry point & main loop
│   ├── lexer/               # Tokenization of input
│   │   ├── lexer.c
│   │   ├── lexer_utils.c
│   │   ├── process_token.c
│   │   └── process_token_utils.c
│   ├── expander/            # Variable & quote expansion
│   │   ├── expander.c
│   │   ├── expand_var.c
│   │   ├── expand_double.c
│   │   ├── expand_single.c
│   │   ├── merger.c
│   │   ├── expander_utils.c
│   │   └── expander_checks.c
│   ├── syntax_check/        # Input validation
│   │   ├── syntax_check.c
│   │   └── var_name_check.c
│   ├── parser/              # Token → command structures
│   │   ├── parser.c
│   │   ├── parsing_utils.c
│   │   ├── parsing_utils_2.c
│   │   └── parsing_utils_3.c
│   ├── executor/            # Command execution engine
│   │   ├── executor.c
│   │   ├── single.c         # Single command execution
│   │   ├── first.c          # First command in pipeline
│   │   ├── next.c           # Middle commands in pipeline
│   │   ├── last.c           # Last command in pipeline
│   │   ├── handle_redir.c   # Redirection handling
│   │   ├── find_path.c      # PATH resolution
│   │   └── ...
│   ├── builtins/            # Built-in command implementations
│   │   ├── echo.c
│   │   ├── cd.c
│   │   ├── pwd.c
│   │   ├── export.c
│   │   ├── unset.c
│   │   ├── env.c
│   │   └── exit.c
│   ├── heredoc/             # Here-document handling
│   │   ├── heredoc.c
│   │   └── heredoc_utils.c
│   └── utils/               # Memory, signals, errors
│       ├── init_memory.c
│       ├── signals.c
│       ├── freeing.c
│       ├── free_memory.c
│       ├── handle_error.c
│       └── print_error_message.c
├── libft/                   # Custom C library (42 libft)
├── heredoc/                 # Heredoc temp file storage
├── Makefile
└── README.md
```

### How It Works

1. **Lexer** — Tokenizes the raw input string into a linked list of tokens (words, pipes, redirections, quotes, variables).
2. **Expander** — Resolves environment variables (`$VAR`), handles quote removal, and merges adjacent tokens.
3. **Syntax Check** — Validates token structure (e.g., no pipe at start, no dangling redirections).
4. **Parser** — Converts the token list into a linked list of `t_command` structures with arguments and redirections.
5. **Executor** — Forks processes, sets up pipes and redirections, resolves paths, and executes commands (builtins run in-process when possible).

---

## Getting Started

### Prerequisites

- **GCC** or **CC** compiler
- **GNU Make**
- **Readline library** (`libreadline-dev`)

#### Install readline (if not already available):

```bash
# macOS (with Homebrew)
brew install readline

# Ubuntu / Debian
sudo apt-get install libreadline-dev

# Fedora
sudo dnf install readline-devel
```

### Build

```bash
# Clone the repository
git clone <repository-url>
cd minishell

# Compile
make

# Run
./minishell
```

### Makefile Targets

| Target | Description |
|--------|-------------|
| `make` / `make all` | Build the project |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and binary |
| `make re` | Full recompile |

---

## Usage

```bash
# Start the shell
./minishell

# You'll see a prompt with your current directory:
/Users/username/minishell$
```

### Examples

```bash
# Simple command
/home/user$ ls -la

# Pipes
/home/user$ cat file.txt | grep "hello" | wc -l

# Redirections
/home/user$ echo "hello world" > output.txt
/home/user$ cat < input.txt >> output.txt

# Heredoc
/home/user$ cat << EOF
> line 1
> line 2
> EOF

# Environment variables
/home/user$ export MY_VAR="Hello"
/home/user$ echo $MY_VAR
Hello

# Exit status
/home/user$ ls nonexistent_file
/home/user$ echo $?
2

# Quotes
/home/user$ echo "Hello $USER"       # Variable expanded
/home/user$ echo 'Hello $USER'       # Literal string

# Exit
/home/user$ exit
```

---

## Testing

### Memory Leak Detection

The project includes a Valgrind configuration for leak checking:

```bash
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --show-error-list=yes \
         --suppressions=readline.supp \
         --trace-children=yes \
         --track-fds=yes \
         ./minishell
```

> **Note:** `readline.supp` is included to suppress known readline library leaks that are not caused by the project code.

---

## Key Concepts Learned

- **Process creation** with `fork()` and `execve()`
- **Inter-process communication** with `pipe()`
- **File descriptor manipulation** with `dup2()`
- **Signal handling** (`SIGINT`, `SIGQUIT`, `SIGPIPE`)
- **Lexical analysis** and **parsing** techniques
- **Memory management** in C (no leaks!)
- **Environment variable** management
- **Heredoc** implementation with temp files

---

## Limitations

This is a learning project and does not implement the full POSIX shell specification. Notable limitations include:

- No logical operators (`&&`, `||`)
- No subshells / command grouping with `()`
- No wildcards / globbing (`*`)
- No job control (`bg`, `fg`, `jobs`)
- No scripting / non-interactive mode

---

## License

This project was developed as part of the 42 School curriculum. Feel free to use it as a reference, but please respect the [42 School's academic integrity rules](https://www.42.fr/).

---

<p align="center">
  Made at <strong>42 Vienna</strong>
</p>

