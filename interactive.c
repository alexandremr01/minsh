#include "interactive.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>


char* read_line();

stringarr *prompt_command(){
    using_history();
    char *line = readline("> ");
    add_history (line);
    return split_string(line, " ");
}

typedef struct chararr {
    char *str;
    int cap, count;
} chararr;

chararr* newchararr(){
    chararr *s = malloc(sizeof(chararr*));
    s->count = 0;
    s->cap = 256;
    s->str = malloc(s->cap*sizeof(char));
    return s;
}

void chararr_append(chararr *s, char c){
    s->str[s->count] = c;
    if (s->count == s->cap - 1){
        s->cap = 2*s->cap;
        s->str = realloc(s->str, s->cap*sizeof(char));
    }
    s->count++;
}

char* read_line() {
    chararr* command = newchararr();
    while(1){
        char character = getchar();
        if (character == '\n'){
            chararr_append(command, '\0');
            break;
        }
        chararr_append(command, character);
    }
    return command->str;
}