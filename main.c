#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "stringarr.h"
#include "job.h"
#include "parser.h"

char* read_line();
char **split_command(char *command);
void initialize_job(command *command);

int main(){
    printf("MINi SHell 0.1.0 by Alexandre Maranhao\n\n");
    while(1){
        printf("> ");
        char *line = read_line();
        char **words = split_command(line);

        job *jobs_head = parse(words);
        if (jobs_head == NULL) {
            continue;
        }

        job *p = jobs_head;
        while (p != NULL){
            if (p->next != NULL){
                int pipefd[2];
                pipe(pipefd);
                p->next->command->input = pipefd[0];
                p->command->output = pipefd[1];
            }
            initialize_job(p->command);  
            p = p->next;      
        }
    }
    return 0;
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

char **split_command(char *command){
    int cap = 5;
    char **parts = malloc(cap * sizeof(char*));
    int i = 0;
    char *token;
    while ((token = strsep(&command, " ")) != NULL){
        parts[i] = token;
        if (i == cap-1){
            cap = 2*cap;
            command = realloc(command, cap*sizeof(char*));
        }
        i++;
    }
    parts[i] = malloc(sizeof(char));
    parts[i][0] = '\0';
    return parts;
}

char* read_line() {
    int cap = 256;
    char *command = malloc(cap);
    int i = 0;
    while(1){
        char character;
        scanf("%c", &character);
        if (character == '\n'){
            command[i] = '\0';
            break;
        }
        command[i] = character;
        if (i == cap-1){
            cap = 2*cap;
            command = realloc(command, cap);
        }
        i++;
    }
    return command;
}