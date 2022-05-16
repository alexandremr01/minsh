#ifndef JOB_H
#define JOB_H

#include <stdlib.h>

#include "stringarr.h"
#include "status.h"

typedef struct job {
    struct program *program_head;
    stringarr *command_line;

    int foreground;
    pid_t pgid;

    struct job *next;
    program_status status;
} job;

job *new_job(struct program *program_head, int foreground);

void free_jobs(job *job);

job *new_job_head();

int job_has_finished(job *job);

int job_is_running(job *job);

int job_has_stopped(job *job);

#endif
