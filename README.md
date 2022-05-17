# minsh: Mini Shell

A minimum shell developed for course CES-33 at ITA, SP, BR.

# Code organization

- `parser`: Parser and Lexer used to parse a command
- `interactive`: Functions related to reading input and displaying information
- `system`: System calls to execute and manage jobs. Interfaces with the OS.
- `types`: Data structures program, job and stringarr

# Features

- Output redirecting (>)
- Input redirecting (<)
- Pipelining (|)
- Running in background (&)
- Signal handling (Ctrl+C, Ctrl+Z)

Obs: Only the first member of a pipeline can redirect input. Only the last member can redirect output.

## Internal commands

- `jobs`: Lists running jobs
- `bg [n]`: Takes job n to background, defaults to 0 
- `fg [n]`: Brings job n to foreground, defaults to 0 

# Preparing the environement

A docker container with the necessary dependencies can be run with `docker compose run main`. Docker server v20.10.14 was used during development.

# Building and Running

Build with `make debug` or `make release`. Run with `./bin/minsh`.

# Example inputs

```sh
/bin/ls | /usr/bin/sort -r | /usr/bin/tail -n 3 > outtest
/bin/sleep 5 &
```

# Testing

Running the script `./test/test-1.sh` will run the minishell with a prepared input and compare the result generated to test-1.output.txt with the test-1.exepected.txt. 

Empty script output means success (real output matches expected).