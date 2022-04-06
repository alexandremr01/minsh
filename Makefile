CC_FLAGS=-Wall -Wextra -Wundef -Wpointer-arith -Wcast-align
INPUT_FILES=main.c interactive.c runner.c types/job.c types/stringarr.c parser.c
CC=gcc

build:
	$(CC) $(CC_FLAGS) $(INPUT_FILES) -o bin/minsh