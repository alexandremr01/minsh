#include "internal.h"
#include "../runner/runner.h"

typedef int (*internal_executor)(stringarr *command_line, job *jobs); // function pointer

char *internal_commands[5] = {"exit", "jobs", "fg", "bg", NULL};

int get_internal_command_index(stringarr *command_line){
    int index = -1;
    if (command_line->count == 0)
        return -1;

    for (int i = 0; internal_commands[i] != NULL; i++){
        if (strcmp(command_line->values[0], internal_commands[i]) == 0) {
            return i;
        }
    }
    return index;
}

int is_internal_command(stringarr *command_line){
    return get_internal_command_index(command_line) != -1;
}

int execute_internal_command(stringarr *command_line, job * jobs){
    if (strcmp(command_line->values[0], "exit") == 0) {
        return 1;
    } else if (strcmp(command_line->values[0], "jobs") == 0){
        display_jobs(jobs);
    } else if (strcmp(command_line->values[0], "bg") == 0){
        resume_job(jobs, 0);
    } else if (strcmp(command_line->values[0], "fg") == 0){
        resume_job(jobs, 1);
    } else {
        printf("Error: %s is not an internal command.", command_line->values[0]);
    }
    return 0;
}
