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

command* append_command(command* current_command, stringarr *args);
command* new_command(stringarr *str);
void free_commands(command *cmd);

#endif