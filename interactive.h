#ifndef INTERACTIVE_H
#define INTERACTIVE_H
#include "types/stringarr.h"

// prompt_command asks the user for input and returns the input as a stringarray
stringarr *prompt_command();

// init_shell prints message and setup handler
void init_shell();

#endif