#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../types/program.h"
#include "../types/job.h"

// execute_programs takes a linked list of programs and executes each of then
void execute_job(job *jobs, job *j);

#endif
