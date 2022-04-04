CC_FLAGS=-Wall -Wextra -Wundef -Wpointer-arith -Wcast-align
INPUT_FILES=main.c runner.c job.c stringarr.c parser.c

build:
	gcc $(CC_FLAGS) $(INPUT_FILES) -o bin/minsh