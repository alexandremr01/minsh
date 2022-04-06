#ifndef RUNNER_H
#define RUNNER_H
#include "types/command.h"

void initialize_command(command *command);
void run_commands(command *command_head);
int validate(command *command_head);

#endif