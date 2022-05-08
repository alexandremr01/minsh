#ifndef INTERNAL_H
#define INTERNAL_H

#include <stdlib.h>
#include <string.h>

#include "../types/stringarr.h"

int is_internal_command(stringarr *command_line);
void execute_internal_command(stringarr *command_line);

#endif

