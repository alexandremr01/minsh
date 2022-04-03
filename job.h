#ifndef JOB_H
#define JOB_H
#include "stringarr.h"

typedef struct job {
    stringarr *args;
    struct job *next;
} job;

job* append_job(job* current_job, stringarr *args);

#endif