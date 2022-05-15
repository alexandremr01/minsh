#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>

#include "stringarr.h"

typedef enum program_status { RUNNING, STOPPED, FINISHED } program_status;

typedef struct program {
    stringarr *args;
    int input, output;
    char *inputFile, *outputFile;
    struct program *next;

    program_status status;
    pid_t pid;
} program;

program *new_program();

void free_programs(program *prg);

#endif