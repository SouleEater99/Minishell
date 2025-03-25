# 🐚 Minishell - As Beautiful as a Shell

**A 42 School project to create a simplified UNIX command-line interpreter**  
*Inspired by bash, written in C with ❤️*

[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Norminette](https://img.shields.io/badge/code%20style-42_Norminette-green.svg)](https://github.com/42School/norminette)

## 🌟 Features

### Mandatory
- **Interactive prompt** with working command history (using GNU readline)
- **Command execution** via PATH or absolute/relative paths
- **Advanced parsing** with single quotes `'`, double quotes `"`, and environment variables (`$VAR`)
- **Redirections**:
  - Input `<` and output `>` redirection
  - Heredoc `<<` delimiter support
  - Append mode `>>`
- **Pipes** (`|`) for multi-command chaining
- **Signal handling** for Ctrl-C, Ctrl-D, and Ctrl-\ (bash-like behavior)
- **Builtins**:
  - `echo` with `-n` option
  - `cd`, `pwd`, `exit`
  - `export`, `unset`, `env` for environment management

### Bonus (Optional)
- **Logical operators** `&&` and `||` with parentheses
- **Wildcard expansion** `*` for current directory
- Enhanced error handling and edge case management

## 🛠️ Installation

1. Clone repository:
   ```bash
   git clone https://github.com/<your-username>/minishell.git
   cd minishell
   
### Compile with Makefile:

```bash
make
```
### Run:

```bash
./minishell
```
Requires GNU readline library (install via package manager if needed)

## 🚀 Usage
```
$> ./minishell
minishell> ls -l | grep .c | wc -l
      42
minishell> echo "Hello $USER" > greeting.txt
minishell> cat << EOF > output.txt
heredoc> Multiline
heredoc> input
heredoc> EOF
```
## 📚 Implementation Details
Language: C (C99 standard)

Memory: Zero-leaks policy with careful allocation/free management

Architecture: Modular design with separate parsing/execution logic

Compliance: Follows 42 School Norm (strict code style guidelines)

Dependencies: Uses readline for line editing and history features

## 🧠 Challenges Overcome
Precise handling of quoting and expansion rules

File descriptor management for complex redirections

Process synchronization in pipes

Signal handling in interactive/non-interactive modes

Environment variable management

## ⚠️ Limitations
Not a full POSIX-compliant shell

Limited built-in commands compared to bash

No tilde expansion or advanced globbing

## 🤝 Contributing
Pull requests welcome! Please follow:

42 School Norm guidelines

Test thoroughly

Document changes

## 📜 License
MIT License - See LICENSE for details

"The best way to predict the future is to implement it." - Inspired by Alan Kay
