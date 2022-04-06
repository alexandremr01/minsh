#include "job.h"
#include <stdlib.h>

job* append_job(job* current_job, stringarr *args){
    job *new_job = malloc(sizeof(job));
    command *new_command = malloc(sizeof(command));
    new_command->args = args;
    new_job->command = new_command;
    current_job->next = new_job;
    return new_job;
}

job* new_job(command *command){
    job *new_job = malloc(sizeof(job));
    new_job->command = command;
    return new_job;
}

command* new_command(stringarr *str){
    command *new_command = malloc(sizeof(command));
    new_command->args = str;
    new_command->input = -1;
    new_command->output = -1;
    return new_command;
}