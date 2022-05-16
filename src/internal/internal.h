#ifndef INTERNAL_H
#define INTERNAL_H

#include <stdlib.h>
#include <string.h>

#include "../types/stringarr.h"
#include "../types/job.h"
#include "../interactive/interactive.h"

int is_internal_command(stringarr *command_line);
int execute_internal_command(stringarr *command_line, job *jobs);

#endif

