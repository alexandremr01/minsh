#include "parser.h"
#include "types/stringarr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define REDIRECT_OUTPUT 1
#define REDIRECT_INPUT 2
#define PIPE 3
#define EOL 4
#define ARG 5

command* parse_line(stringarr *words, int *current_word);
int token_type(stringarr *words, int current_word);
command* parse_command(stringarr *words, int *current_word);

int is_terminator(int token_type){
    return token_type==PIPE || token_type == EOL;
}

char* parse_filename(stringarr *words, int *current_word){
    if (token_type(words, *current_word) != ARG) {
        printf("Expected a filename\n");
        return NULL;
    }
    char *input = words->values[*current_word];
    (*current_word)++;

    return input;
}

command* parse_command(stringarr *words, int *current_word){
    stringarr *args = new_stringarr();
    while (token_type(words, *current_word) == ARG) {
        stringarr_append(args, words->values[*current_word]);
        (*current_word)++;
    }

    command *cmd = new_command(args);
    while ( !is_terminator(token_type(words, *current_word)) ) {
        int token = token_type(words, *current_word);
        if (token == REDIRECT_INPUT || token == REDIRECT_OUTPUT) {
            (*current_word)++;
            char *filename = parse_filename(words, current_word);
            if (filename == NULL){
                return NULL;
            }
            if (token == REDIRECT_INPUT)
                cmd->inputFile = filename;
            else cmd->outputFile = filename;
        }
    }

    return cmd;
}

command* parse(stringarr *words){
    int current_word = 0;
    return parse_line(words, &current_word);
}

command* parse_line(stringarr *words, int *current_word){
    command *cmd = parse_command(words, current_word);
    if (cmd->args->count == 0){
        printf("Error: empty command\n");
        return NULL;
    }
    if (token_type(words, *current_word) == PIPE){
        (*current_word)++;
        command *next_cmd = parse_line(words, current_word);
        if (next_cmd == NULL) {
            return NULL;
        }
        cmd->next = next_cmd;
    } else if (token_type(words, *current_word) != EOL){
        printf("Syntax error\n");
        return NULL;
    }
    return cmd;
}

int token_type(stringarr *words, int current_word){
    char *next = words->values[current_word];
    if ( strcmp(next, ">") == 0 ) return REDIRECT_OUTPUT;
    else if ( strcmp(next, "<") == 0 ) return REDIRECT_INPUT;
    else if ( strcmp(next, "|") == 0 ) return PIPE;
    else if ( strcmp(next, "\0") == 0) return EOL;
    else if ( strcmp(next, "\n") == 0) return EOL;
    return ARG;
}