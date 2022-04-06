#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "types/stringarr.h"
#include "types/job.h"
#include "parser.h"
#include "runner.h"

char* read_line();
stringarr *prompt_command();

int main(){
    printf("MINi SHell 0.1.0 by Alexandre Maranhao\n\n");
    while(1){
        stringarr *command_line = prompt_command();

        job *jobs_head = parse(command_line);
        if (jobs_head == NULL) {
            continue;
        }

        if (validate(jobs_head) != 0) continue;

        run_jobs(jobs_head);
    }
    return 0;
}

stringarr *prompt_command(){
    printf("> ");
    char *line = read_line();
    return split_string(line, " ");
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