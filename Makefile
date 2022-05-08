CC        := gcc
CFLAGS    := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align -pedantic -std=c11 -D_XOPEN_SOURCE=500 -Werror=vla
LDFLAGS   := -lreadline
BUILD     := ./bin

SOURCEDIR := $(CURDIR)/src

SOURCEFILES  := main.c interactive/interactive.c runner/runner.c \
        		types/program.c types/stringarr.c \
        		parser/parser.c parser/lexer.c

SRC :=  $(foreach file, $(SOURCEFILES), $(addprefix $(SOURCEDIR)/, $(file)))

.PHONY: all build clean debug release info

all: build

build:
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(BUILD)/minsh

debug: CFLAGS += -DDEBUG -g -fsanitize=address
debug: all

release: CFLAGS += -O2
release: all

clean:
	-@rm -rvf $(BUILD)/*