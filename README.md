# minsh Mini Shell

# Preparing the environement

If you are not running in a Linux environment, it is recommended to use `docker`.

Run with `docker compose run main`.

# Building and Running

Build with

```sh
mkdir bin
make build
./bin/minsh
```

# Features

- Output redirecting (>)
- Input redirecting (<)
- Pipelining (|)

Obs: Only the first member of a pipeline can redirect input. Only the last member can redirect output.

# Example input

```sh
/bin/ls | /usr/bin/sort -r | /usr/bin/tail -n 3 > outtest
```

# Automated end-to-end testing

```shell
./test/test-1.sh
```