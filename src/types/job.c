#include "job.h"
#include "program.h"

job *new_job(struct program *program_head, int foreground){
    job *new_job = malloc(sizeof(job));
    new_job->program_head = program_head;
    new_job->foreground = foreground;
    new_job->next = NULL;
    return new_job;
}

void free_jobs(job *job){
    if (job == NULL)
        return;
    free_programs(job->program_head);
    free_jobs(job->next);
    free(job);
}
