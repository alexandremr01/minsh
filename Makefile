CC_FLAGS=-Wall -Wextra -Wundef -Wpointer-arith -Wcast-align
INPUT_FILES=main.c interactive.c runner.c \
    types/command.c types/stringarr.c types/chararr.c \
    parser/parser.c parser/lexer.c

CC=gcc

build:
	$(CC) $(CC_FLAGS) $(INPUT_FILES) -lreadline -g -o bin/minsh