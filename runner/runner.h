#ifndef RUNNER_H
#define RUNNER_H

#include "../types/command.h"

// execute_commands takes a linked list of commands and executes each of then
void execute_commands(command *command_head);

// validate takes a linked list of commands and check for semantic errors
int validate(command *command_head);

#endif
