#ifndef JOB_SERVICES_H
#define JOB_SERVICES_H

#include "../types/program.h"
#include "../types/job.h"

// validate takes a linked list of programs and check for semantic errors
int validate(job *j);

// check_running_programs checks whether any job has terminated or stopped
void check_running_programs(job *jobs);

// resume_job is used for bringing back stopped or background jobs
void resume_job(job *jobs, int foreground, job *j);

// wait_for_job is used for letting the job as foreground
void wait_for_job(job *jobs, job *j);

#endif
