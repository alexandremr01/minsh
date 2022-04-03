#include "parser.h"
#include "stringarr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define REDIRECT_OUTPUT 1
#define REDIRECT_INPUT 2
#define PIPE 3
#define EOL 4
#define ARG 5

int token_type(char **words, int current_word);
command* parse_command(char **words, int *current_word);

int is_terminator(int token_type){
    return token_type==PIPE || token_type == EOL;
}

command* parse_command(char **words, int *current_word){
    stringarr *args = new_stringarr();
    while (token_type(words, *current_word) == ARG) {
        stringarr_append(args, words[*current_word]);
        (*current_word)++;
    }

    // while ( !is_terminator(token_type(words, *current_word)) ) {
    //     if ( token_type(words, *current_word) == REDIRECT_INPUT ){
    //         // parse_out();
    //     } else if ( token_type(words, *current_word) == REDIRECT_OUTPUT ){
    //         // parse_in();
    //     }
    // }

    return new_command(args);
}

job* parse(char **words){
    int current_word = 0;
    return parse_line(words, &current_word);
}

job* parse_line(char **words, int *current_word){
    command *command = parse_command(words, current_word);
    if (command->args->count == 0){
        printf("Error: empty command\n");
        return NULL;
    }
    job *current_job = new_job(command);
    if (token_type(words, *current_word) == PIPE){
        (*current_word)++;
        job *command = parse_line(words, current_word);
        if (command == NULL) {
            return NULL;
        }
        current_job->next = command;
    } else if (token_type(words, *current_word) != EOL){
        printf("Syntax error\n");
        return NULL;
    }
    return current_job;
}

int token_type(char **words, int current_word){
    char *next = words[current_word];
    if ( strcmp(next, ">") == 0 ) return REDIRECT_OUTPUT;
    else if ( strcmp(next, "<") == 0 ) return REDIRECT_INPUT;
    else if ( strcmp(next, "|") == 0 ) return PIPE;
    else if ( strcmp(next, "\0") == 0) return EOL;
    else return ARG;
}