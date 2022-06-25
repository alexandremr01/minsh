#include "executor.h"
#include "job_services.h"

#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>

int execute_program(program *program, int foreground, job *j) {
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
        if (j->pgid == -1) {
            j->pgid = pid;
            if (foreground) tcsetpgrp(STDIN_FILENO, j->pgid);
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
        if (j->pgid == -1) {
            j->pgid = cpid;
            if (foreground) tcsetpgrp(STDIN_FILENO, j->pgid);
        }
        setpgid(j->pgid, cpid);

        if (program->input != -1) close(program->input);
        if (program->output != -1) close(program->output);
    }
    return 0;
}

void execute_job(job *jobs, job *j) {
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

        int result = execute_program(p, foreground, j);
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


