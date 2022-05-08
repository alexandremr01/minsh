#include "runner.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <termios.h>
#include <signal.h>

extern pid_t my_pid;
extern struct termios shell_tmodes;

int execute(program *program);

int validate(program *programs_head) {
    program *p = programs_head;
    while (p != NULL) {
        if (p->inputFile != NULL && p != programs_head->next) {
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

void execute_programs(program *programs_head) {
    program *p = programs_head->next;
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

        int result = execute(p);
        if (result != 0) {
            if (p->next != NULL && p->next->input != -1) close(p->next->input);
            return;
        }
        p = p->next;
    }
}

int execute(program *program) {
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

        int pid = getpid();
        setpgid(pid, pid);
        tcsetpgrp (STDIN_FILENO, pid);

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

        setpgid(cpid, cpid);
        tcsetpgrp(STDIN_FILENO, cpid);
        if (program->input != -1) close(program->input);
        if (program->output != -1) close(program->output);

        int child_status;
        waitpid(cpid, &child_status, WUNTRACED);
        int mypid = getpid();
        tcsetpgrp(STDIN_FILENO, mypid);
        tcsetattr(STDIN_FILENO, TCSADRAIN, &shell_tmodes);

        if (WEXITSTATUS(child_status) == EXIT_FAILURE)
            return -1;
    }
    return 0;
}
