#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "../types/stringarr.h"
#include "lexer.h"

command* parse_line(lexer *lex);
command* parse_command(lexer *lex);
int is_terminator(int token_type);

command* parse(stringarr *words){
    if (words->count == 0)
        return NULL;
    lexer *lex = new_lexer(words);
    command* parsed_commands = parse_line(lex);
    free(lex);
    return parsed_commands;
}

command* parse_line(lexer *lex){
    command *cmd = parse_command(lex);
    if (cmd->args->count == 1) {
        printf("Error: empty command\n");
        return NULL;
    }
    int tok = lex_current_token(lex);
    lex_next_token(lex);
    if (tok == PIPE) {
        command *next_cmd = parse_line(lex);
        if (next_cmd == NULL) {
            return NULL;
        }
        cmd->next = next_cmd;
        return cmd;
    } else if (tok == EOL) {
        return cmd;
    }

    printf("Syntax error\n");
    return NULL;
}

char* parse_filename(lexer *lex){
    if (lex_current_token(lex) != ARG) {
        printf("Expected a filename\n");
        return NULL;
    }
    char *input = lex_get_string(lex);
    lex_next_token(lex);

    return input;
}

command* parse_command(lexer *lex){
    stringarr *args = new_stringarr();
    while (lex_current_token(lex) == ARG) {
        char *s = lex_get_string(lex);
        stringarr_append(args, s);
        lex_next_token(lex);
    }
    stringarr_append(args, NULL);

    command *cmd = new_command(args);
    while ( !is_terminator(lex_current_token(lex))) {
        int token = lex_current_token(lex);
        if (token == REDIRECT_INPUT || token == REDIRECT_OUTPUT) {
            lex_next_token(lex);
            char *filename = parse_filename(lex);
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

int is_terminator(int token_type){
    return token_type==PIPE || token_type == EOL;
}
