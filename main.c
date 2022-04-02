#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

char* read_command();
char **split_command(char *command);
void initialize_job(char **argv);

int main(){
    while(1){
        printf("> ");
        char *command = read_command();
        char **parts = split_command(command);
        
        int j = 0;
        int cap = 5;
        char **argv = malloc(cap*sizeof(char*));
        int argc = 0;
        for (j=0; parts[j][0] != '\0'; j++){
            if ( strcmp(parts[j], "|") == 0 ){
                initialize_job(argv);
                argv = malloc(cap*sizeof(char*));
                argc = 0;
                continue;
            }

            argv[argc] = parts[j];
            if (argc == cap - 1){
                cap = 2*cap;
                argv = realloc(argv, cap*sizeof(char*));
            }
            argc++;
        }
        initialize_job(argv);        
    }
    return 0;
}

void initialize_job(char **argv){
    pid_t cpid = fork();
    char *newenviron[] = { NULL };

    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {   
        int result = execve(argv[0], argv, newenviron);
        if (result == -1) {
            printf("Could not run %s\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    } else{
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

char* read_command() {
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