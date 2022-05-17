#include "job_services.h"
#include "../interactive/interactive.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>

extern struct termios shell_tmodes;

int validate(job *j) {
    program *p = j->program_head;
    while (p != NULL) {
        if (p->inputFile != NULL && p != j->program_head->next) {
            printf("Invalid input: Only the first program in a pipeline can redirect input\n");
            return -1;
        }
        if (p->outputFile != NULL && p->next != NULL) {
            printf("Invalid input: Only the last program in a pipeline can redirect output\n");
            return -1;
        }
        p = p->next;
    }
    return 0;
}

void wait_for_job(job *jobs, job *j){
    int child_status;
    while (job_is_running(j)) {
        pid_t finished_pid = waitpid(-1, &child_status, WUNTRACED);
        if (finished_pid == -1 && errno == ECHILD) continue;
        program *p = j->program_head->next;
        while (p!=NULL) {
            if (p->pid == finished_pid)
                break;
            p = p->next;
        }
        if (p == NULL) continue;
        if (WIFSTOPPED(child_status)) p->status = STOPPED;
        if (WIFEXITED(child_status) || WIFSIGNALED(child_status)) p->status = FINISHED;
    }

    int mypid = getpid();
    tcsetpgrp(STDIN_FILENO, mypid);
    tcsetattr(STDIN_FILENO, TCSADRAIN, &shell_tmodes);

    if(job_has_finished(j)) {
        for (job *aux = jobs; aux; aux = aux->next){
            if(aux->next == j){
                aux->next = j->next;
            }
        }
        free_jobs(j);
    } else if(job_has_stopped(j)) {
        j->status = STOPPED;
    }
}

void finish_job(job *jobs, pid_t pid) {
    job *selected_job;
    for (job *j = jobs->next; j; j = j->next){
        for (program *p = j->program_head->next; p; p = p->next) {
            if (p->pid == pid) {
                p->status = FINISHED;
                selected_job = j;
            }
        }
    }

    if (!job_is_running(selected_job)){
        for (job *j = jobs; j; j = j->next) {
            if (j->next == selected_job){
                j->next = selected_job->next;
                notify_ended(selected_job);
                free_jobs(selected_job);
                break;
            }
        }
    }
}

void check_running_programs(job *jobs){
    int cont = 1;
    while (cont){
        int status;
        pid_t pid = waitpid (-1, &status, WUNTRACED|WNOHANG);
        int err = errno;
        if (pid == -1 && err == ECHILD) break;
        if (pid == 0) break;
        else if (pid == -1) {
            printf("Could not check children: err %d", err);
            return;
        }

        if (WIFEXITED(status)) { finish_job(jobs, pid); }
    }
}

void resume_job(job *jobs, int foreground, job *j){
    if (j == NULL) {
        printf("No job to resume\n");
        return;
    }
    j->status = RUNNING;
    for (program *p = j->program_head->next; p; p = p->next){
        kill(p->pid, SIGCONT);
        p->status = RUNNING;
    }
    if(foreground){
        tcsetpgrp (STDIN_FILENO, j->pgid);
        wait_for_job(jobs, j);
    }
}
