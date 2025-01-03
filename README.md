# Minishell Project

## Brief Explanation

The Minishell project is a simple shell implementation that mimics the behavior of the Bash shell. It allows users to execute commands, handle redirections, pipes, and environment variables, and supports built-in commands such as `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`. The project aims to provide a basic understanding of how a shell works and how to implement its core functionalities.

## General Requirements

- You may only use one global variable. Be thoughtful and ready to explain its use.
- Display a prompt when waiting for a new command.
- Search and execute the correct executable based on the PATH variable or by using relative/absolute paths, similar to bash.
- Implement the following built-in commands, with their respective specifications:
  - `echo` with the `-n` option.
  - `cd` with only a relative or absolute path.
  - `pwd` without options.
  - `export` without options.
  - `unset` without options.
  - `env` without options or arguments.
  - `exit` without options.
- Use `;` to separate commands, similar to bash.
- Single (`'`) and double (`"`) quotes should work like in bash, except for multi-line commands.
- Handle redirections `<`, `>`, and `>>` like bash, except for file descriptor aggregation.
- Implement pipe (`|`) functionality like bash, except for multi-line commands.
- Handle environment variables (e.g., `$VARIABLE`) like bash.
- The `$?` variable should function like in bash, reflecting the exit status of the last command.
- Implement the following behaviors:
  - `ctrl-C`, `ctrl-D`, and `ctrl-\` should behave like in bash.
  - Use up and down arrow keys to navigate through command history (mandatory).
  - Commands in the history can be edited, at least for classic lines (not the entire history).

## Instructions on How to Use

1. Clone the repository:
   ```sh
   git clone https://github.com/nuketayev/mini-shell.git
   cd mini-shell
   ```

2. Build the project:
   ```sh
   make
   ```

3. Run the shell:
   ```sh
   ./minishell
   ```

4. Use the shell to execute commands, handle redirections, pipes, and environment variables, and use built-in commands.

## Example of Output

```sh
$ echo Hello, World!
Hello, World!
$ cd /tmp
$ pwd
/tmp
$ export MY_VAR=42
$ echo $MY_VAR
42
$ unset MY_VAR
$ echo $MY_VAR

$ env
<list of environment variables>
$ exit
```
