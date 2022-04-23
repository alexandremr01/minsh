#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>

#include "stringarr.h"

typedef struct command {
    stringarr *args;
    int input, output;
    char *inputFile, *outputFile;
    struct command *next;
} command;

command *new_command();

void free_commands(command *cmd);

#endif