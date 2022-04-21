# minsh: Mini Shell

A minimum shell developed for course CES-33 at ITA, SP, BR.

# Features

- Output redirecting (>)
- Input redirecting (<)
- Pipelining (|)
- Signal handling for SIGINT

Obs: Only the first member of a pipeline can redirect input. Only the last member can redirect output.

# Preparing the environement

If you are not running in a Linux environment, it is recommended to use `docker`.

Run with `docker compose run main`.

# Building and Running

Build with `make build`. Run with `./bin/minsh`.

# Example inputs

```sh
/bin/ls | /usr/bin/sort -r | /usr/bin/tail -n 3 > outtest
```

# Automated end-to-end testing

```shell
./test/test-1.sh
```