#include "job.h"
#include <stdlib.h>

job* append_job(job* current_job, stringarr *args){
    job *new_job = malloc(sizeof(job));
    new_job->args = args;
    current_job->next = new_job;
    return new_job;
}