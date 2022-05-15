#include "job.h"

job *new_job(struct program *program_head, int foreground){
    job *new_job = malloc(sizeof(job));
    new_job->program_head = program_head;
    new_job->foreground = foreground;
    return new_job;
}

void free_jobs(job *job){
    return;
}
