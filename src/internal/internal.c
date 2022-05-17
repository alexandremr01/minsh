#include "internal.h"

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

int isNumber(char *s)
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
            return 0;
    }
    return 1;
}

job *get_job(stringarr *command_line, job * jobs){
    int job_number = 0;
    if (command_line->count == 2) {
        if (!isNumber(command_line->values[1])){
            printf("Parameter must be number of job in jobs list.\n");
            return NULL;
        }
        job_number = atoi(command_line->values[1]);
    }
    job *j = jobs;
    for (int i = 0; i <= job_number && j; i++, j = j->next);
    return j;
}

int execute_internal_command(stringarr *command_line, job * jobs){
    if (strcmp(command_line->values[0], "exit") == 0) {
        return 1;
    } else if (strcmp(command_line->values[0], "jobs") == 0){
        display_jobs(jobs);
    } else if (strcmp(command_line->values[0], "bg") == 0){
        job *j = get_job(command_line, jobs);
        resume_job(jobs, 0, j);
    } else if (strcmp(command_line->values[0], "fg") == 0){
        job *j = get_job(command_line, jobs);
        resume_job(jobs, 1, j);
    } else {
        printf("Error: %s is not an internal command.", command_line->values[0]);
    }
    return 0;
}

