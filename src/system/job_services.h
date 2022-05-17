#ifndef JOB_SERVICES_H
#define JOB_SERVICES_H

#include "../types/program.h"
#include "../types/job.h"

// validate takes a linked list of programs and check for semantic errors
int validate(job *j);

void check_running_programs(job *jobs);

void resume_job(job *jobs, int foreground, job *j);

void wait_for_job(job *jobs, job *j);

#endif
