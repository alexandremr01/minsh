#ifndef RUNNER_H
#define RUNNER_H
#include "job.h"

void initialize_job(command *command);
void run_jobs(job *jobs_head);
int validate(job *jobs_head);

#endif