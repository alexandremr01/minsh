#ifndef RUNNER_H
#define RUNNER_H

#include "../types/program.h"
#include "../types/job.h"

// execute_programs takes a linked list of programs and executes each of then
void execute_programs(job *jobs, job *j);

// validate takes a linked list of programs and check for semantic errors
int validate(job *j);

void validate_running_programs(job *jobs);

void resume_job(job *jobs, int foreground);

#endif
