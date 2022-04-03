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
stringarr* parse_command(char **words, int *current_word);

stringarr* parse_command(char **words, int *current_word){
    stringarr *args = new_stringarr();
    while (token_type(words, *current_word) == ARG) {
        stringarr_append(args, words[*current_word]);
        (*current_word)++;
    }
    // char *next = words[*current_word];
    // if ( strcmp(next, ">") == 0 ){
    //     parse_out();
    // } else if ( strcmp(next, "<") == 0 ){
    //     parse_in();
    // }
    // return new_command(args);
    return args;
}

job* parse_line(char **words){
    int current_word = 0;
    job *jobs_head = malloc(sizeof(job));
    job *current_job = jobs_head;
    while (1) {
        stringarr *command = parse_command(words, &current_word);
        if (command->count == 0){
            printf("Error: empty command\n");
            return NULL;
        }
        current_job = append_job(current_job, command);
        if (token_type(words, current_word) == PIPE){
            current_word++;
            continue;
        } else if (token_type(words, current_word) == EOL){
            break;
        } else {
            printf("Syntax error\n");
            return NULL;
        }
    };
    return jobs_head;
}

int token_type(char **words, int current_word){
    char *next = words[current_word];
    if ( strcmp(next, ">") == 0 ) return REDIRECT_OUTPUT;
    else if ( strcmp(next, "<") == 0 ) return REDIRECT_INPUT;
    else if ( strcmp(next, "|") == 0 ) return PIPE;
    else if ( strcmp(next, "\0") == 0) return EOL;
    else return ARG;
}