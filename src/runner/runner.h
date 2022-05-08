#ifndef RUNNER_H
#define RUNNER_H

#include "../types/program.h"

// execute_programs takes a linked list of programs and executes each of then
void execute_programs(program *commaprogram_headnd_head);

// validate takes a linked list of programs and check for semantic errors
int validate(program *program_head);

#endif
