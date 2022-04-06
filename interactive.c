#include "interactive.h"
#include <stdlib.h>
#include <stdio.h>

char* read_line();

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