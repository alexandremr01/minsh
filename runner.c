#include "runner.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void run_jobs(job *jobs_head){
    job *p = jobs_head;
    while (p != NULL){
        if (p->next != NULL){
            int pipefd[2];
            pipe(pipefd);
            p->next->command->input = pipefd[0];
            p->command->output = pipefd[1];
        }
        if (p->command->outputFile != NULL) {
            int fd = open(p->command->outputFile, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG| S_IRWXO);
            if (fd == -1){
                printf("Could not open file %s, error %d\n", p->command->outputFile, errno);
                break;
            }
            p->command->output = fd;
        } 
        if (p->command->inputFile != NULL) {
            int fd = open(p->command->inputFile, O_RDONLY);
            if (fd == -1){
                printf("Could not open file %s, error %d\n", p->command->outputFile, errno);
                break;
            }
            p->command->input = fd;
        } 
        
        initialize_job(p->command);  
        p = p->next;      
    }
}
void initialize_job(command *command){
    pid_t cpid = fork();
    char *newenviron[] = { NULL };

    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {   
        if (command->input != -1){
            dup2(command->input, STDIN_FILENO);
            close(command->input);
        }
        if (command->output != -1){
            dup2(command->output, STDOUT_FILENO);
            close(command->output);
        }
        int result = execve(command->args->argv[0], command->args->argv, newenviron);
        if (result == -1) {
            printf("Could not run %s\n", command->args->argv[0]);
            exit(EXIT_FAILURE);
        }
    } else{
        close(command->input);
        close(command->output);
        wait(NULL);
    }
}
