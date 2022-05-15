#ifndef RUNNER_H
#define RUNNER_H

#include "../types/program.h"
#include "../types/job.h"

// execute_programs takes a linked list of programs and executes each of then
void execute_programs(job *j);

// validate takes a linked list of programs and check for semantic errors
int validate(job *j);

#endif
