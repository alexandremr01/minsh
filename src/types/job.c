#include "job.h"
#include "program.h"

job *new_job(struct program *program_head, int foreground) {
    job *new_job = malloc(sizeof(job));
    new_job->program_head = program_head;
    new_job->foreground = foreground;
    new_job->next = NULL;
    new_job->pgid = -1;
    return new_job;
}

job *new_job_head() {
    job *new_job = malloc(sizeof(job));
    new_job->program_head = NULL;
    new_job->foreground = 0;
    new_job->next = NULL;
    new_job->command_line = NULL;
    return new_job;
}

void free_jobs(job *job) {
    if (job == NULL)
        return;
    if (job->program_head != NULL)
        free_programs(job->program_head);
    if (job->command_line != NULL)
        stringarr_free(job->command_line);
    free(job);
}

int job_has_finished(job *job) {
    program *p = job->program_head->next;
    while (p != NULL) {
        if (p->status != FINISHED)
            return 0;
        p = p->next;
    }
    return 1;
}

int job_has_stopped(job *job) {
    program *p = job->program_head->next;
    while (p != NULL) {
        if (p->status != STOPPED)
            return 0;
        p = p->next;
    }
    return 1;
}

int job_is_running(job *job) {
    program *p = job->program_head->next;
    while (p != NULL) {
        if (p->status == RUNNING)
            return 1;
        p = p->next;
    }
    return 0;
}