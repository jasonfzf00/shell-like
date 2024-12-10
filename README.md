# JShell - Custom Command Line Interface

JShell is a lightweight, feature-rich shell-like CLI written in C. It provides essential shell functionalities with modern features like command history and tab completion.

## Features

### Core Functionality
- Command execution with PATH resolution
- Support for absolute and relative paths
- Built-in `cd` command with home directory (`~`) support
- Command history navigation
- Tab completion for files and directories

### Compilation
Standard compilation:

```sh
make
```

For ARM64 systems:

```sh
gcc -Wall -Werror -Wextra -pedantic *.c -o jsh
```

## Usage

### Starting the Shell

```sh
./jsh
```

### Basic Commands
- Execute programs: `ls`, `pwd`, etc.
- Change directory: `cd [directory]`
- Go to home directory: `cd ~` or `cd`
- Exit the shell: `exit`

## Project Structure
- `main.c`: Core shell loop and process management
- `command.c`: Command parsing and execution
- `file_loc.c`: Path resolution and file location
- `user_input.c`: Command history management
- `tab_completion.c`: Tab completion functionality
- `main.h`: Header file with function declarations
- `Makefile`: Build configuration

## Limitations
- Limited to basic command execution
- No support for complex shell features (pipes, redirections)
- History is not preserved between sessions
- Tab completion limited to current directory

## License
This project is open source and available under the MIT License.