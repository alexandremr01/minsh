#ifndef JOB_H
#define JOB_H
#include "stringarr.h"

typedef struct command_parameters {
    stringarr *args;
    int input, output;
    char *inputFile, *outputFile;
} command;

typedef struct job {
    command *command;
    struct job *next;
} job;

job* append_job(job* current_job, stringarr *args);
job* new_job(command *command);

command* new_command(stringarr *str);
#endif