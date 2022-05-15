#ifndef JOB_H
#define JOB_H

#include <stdlib.h>

#include "stringarr.h"

typedef struct job {
    struct program *program_head;

    int foreground;
    pid_t gpid;

    struct job *next;
} job;

job *new_job(struct program *program_head, int foreground);

void free_jobs(job *job);

#endif