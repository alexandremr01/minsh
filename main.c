#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

char* read_line();
char **split_command(char *command);
void initialize_job(char **argv);

typedef struct job {
    char **argv;
    struct job *next;
} job;

job* append_job(job* current_job, char **argv){
    job *new_job = malloc(sizeof(job));
    new_job->argv = argv;
    current_job->next = new_job;
    return new_job;
}

int main(){
    printf("MINi SHell 0.1.0 by Alexandre Maranhao\n\n");
    while(1){
        printf("> ");
        char *line = read_line();
        char **words = split_command(line);
        
        int current_word = 0;
        int fail=0;
        
        job *jobs_head = malloc(sizeof(job));
        job *current_job = jobs_head;
        int argv_cap = 5;
        char **argv = malloc(argv_cap*sizeof(char*));
        int argc = 0;
        while (words[current_word][0] != '\0'){
            if ( strcmp(words[current_word], "|") == 0 ){
                if (argc == 0){
                    printf("Unexpected pipe |\n");
                    fail = 1;
                    break;
                }
                current_job = append_job(current_job, argv);
                argv_cap = 5;
                argv = malloc(argv_cap*sizeof(char*));
                argc = 0;
            } else {
                argv[argc] = words[current_word];
                if (argc == argv_cap - 1){
                    argv_cap = 2*argv_cap;
                    argv = realloc(argv, argv_cap*sizeof(char*));
                }
                argc++;
            }
            current_word++;
        }
        if (fail) continue;
        current_job = append_job(current_job, argv);

        job *p = jobs_head->next;
        while (p != NULL){
            initialize_job(p->argv);  
            p = p->next;      
        }
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