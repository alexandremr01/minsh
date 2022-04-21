#include "runner.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

extern pid_t current_foreground_process;

int initialize_command(command *command);

int validate(command *commands_head){
    command *p = commands_head;
    while (p != NULL){
        if (p->inputFile != NULL && p != commands_head){
            printf("Invalid input: Only the first command in a pipeline can redirect input\n");
            return -1;
        }
        if (p->outputFile != NULL && p->next != NULL){
            printf("Invalid input: Only the last command in a pipeline can redirect output\n");
            return -1;
        }
        p = p->next;
    }
    return 0;
}

void run_commands(command *commands_head){
    command *p = commands_head;
    while (p != NULL){
        if (p->next != NULL){
            int pipefd[2];
            pipe(pipefd);
            p->next->input = pipefd[0];
            p->output = pipefd[1];
        }
        if (p->outputFile != NULL) {
            int fd = open(p->outputFile, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG| S_IRWXO);
            if (fd == -1){
                printf("Could not open file %s, error %d\n", p->outputFile, errno);
                break;
            }
            p->output = fd;
        } 
        if (p->inputFile != NULL) {
            int fd = open(p->inputFile, O_RDONLY);
            if (fd == -1){
                printf("Could not open file %s, error %d\n", p->outputFile, errno);
                break;
            }
            p->input = fd;
        } 
        
        int result = initialize_command(p);
        if (result != 0) {
            if(p->next != NULL && p->next->input != -1) close(p->next->input);
            return;
        }
        p = p->next;      
    }
}

int initialize_command(command *command){
    pid_t cpid = fork();
    char *newenviron[] = { NULL };

    if (cpid == -1) { // fork error
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (cpid == 0) { // child process
        int std_output = dup(STDOUT_FILENO);
        if (command->input != -1){
            dup2(command->input, STDIN_FILENO);
            close(command->input);
        }
        if (command->output != -1){
            dup2(command->output, STDOUT_FILENO);
            close(command->output);
        }
        int result = execve(command->args->values[0], command->args->values, newenviron);
        if (result == -1) {
            int error = errno;
            dup2(std_output, STDOUT_FILENO);
            printf("Could not run %s: error %d\n", command->args->values[0], error);
            exit(EXIT_FAILURE);
        }
    } else { // parent process
        current_foreground_process = cpid;
        close(command->input);
        close(command->output);
        int child_status;
        wait(&child_status);
        if (WEXITSTATUS(child_status) == EXIT_FAILURE) {
            return -1;
        }
    }
    return 0;
}
