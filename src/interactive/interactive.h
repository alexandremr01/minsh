#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "../types/stringarr.h"
#include "../types/job.h"

// prompt_command asks the user for input and returns the input as a stringarr
stringarr *prompt_command();

// init_shell prints message and setup interruption handler
void init_shell();

// display_jobs print all jobs
void display_jobs(job *jobs);

// notify_ended sends a notification
void notify_ended(job *j);

#endif
