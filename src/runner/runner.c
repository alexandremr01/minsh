#include "runner.h"
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

int execute(program *program, int foreground, job *j);

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

void execute_programs(job *jobs, job *j) {
    program *p = j->program_head->next;
    int foreground = j->foreground;
    while (p != NULL) {
        if (p->next != NULL) {
            int pipefd[2];
            int result = pipe(pipefd);
            if (result == -1) {
                printf("Could not create pipeline, error %d\n", errno);
                break;
            }
            p->next->input = pipefd[0];
            p->output = pipefd[1];
        }
        if (p->outputFile != NULL) {
            int fd = open(p->outputFile, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
            if (fd == -1) {
                printf("Could not open file %s, error %d\n", p->outputFile, errno);
                break;
            }
            p->output = fd;
        }
        if (p->inputFile != NULL) {
            int fd = open(p->inputFile, O_RDONLY);
            if (fd == -1) {
                printf("Could not open file %s, error %d\n", p->outputFile, errno);
                break;
            }
            p->input = fd;
        }

        int result = execute(p, foreground, j);
        if (result != 0) {
            if (p->next != NULL && p->next->input != -1) close(p->next->input);
            return;
        }
        p = p->next;
    }

    j->status = RUNNING;
    j->next = jobs->next;
    jobs->next = j;

    if (foreground) wait_for_job(jobs, j);
}



int execute(program *program, int foreground, job *j) {
    pid_t cpid = fork();
    char *newenviron[] = {NULL};

    if (cpid == -1) { // fork error
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (cpid == 0) { // child process
        // return to default signal handlers
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);

        pid_t pid = getpid();
        if (j->pgid == -1){
            j->pgid = pid;
            if (foreground) tcsetpgrp (STDIN_FILENO, j->pgid);
        }
        setpgid(j->pgid, pid);

        int std_output = dup(STDOUT_FILENO);
        if (program->input != -1) {
            dup2(program->input, STDIN_FILENO);
            close(program->input);
        }
        if (program->output != -1) {
            dup2(program->output, STDOUT_FILENO);
            close(program->output);
        }
        int result = execve(program->args->values[0], program->args->values, newenviron);
        if (result == -1) {
            int error = errno;
            dup2(std_output, STDOUT_FILENO);
            printf("Could not run %s: error %d\n", program->args->values[0], error);
            exit(EXIT_FAILURE);
        }
    } else { // parent process
        program->status = RUNNING;
        program->pid = cpid;
        if (j->pgid == -1){
            j->pgid = cpid;
            if (foreground) tcsetpgrp (STDIN_FILENO, j->pgid);
        }
        setpgid(j->pgid, cpid);

        if (program->input != -1) close(program->input);
        if (program->output != -1) close(program->output);
    }
    return 0;
}

void notify_ended(job *j){
    printf("Job ");
    for (int i=0; i<j->command_line->count; i++)
        printf("%s ", j->command_line->values[i]);
    printf("ended \n");
}

void finish(job *jobs, pid_t pid) {
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

void validate_running_programs(job *jobs){
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

        if (WIFEXITED(status)) { finish(jobs, pid); }
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
