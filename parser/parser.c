#include "parser.h"

int parse_line(command *target, lexer *lex);

int is_terminator(int token_type);

int parse_command(command *cmd, lexer *lex);

int parse(command *head, stringarr *words) {
    if (words->count == 0)
        return 0;

    lexer *lex = new_lexer(words);
    head->next = new_command();
    int result = parse_line(head->next, lex);
    free(lex);

    if (result != 0)
        return -1;
    return 0;
}

int parse_line(command *target, lexer *lex) {
    int result = parse_command(target, lex);
    if (result == -1)
        return -1;
    if (target->args->count == 1) {
        printf("Error: empty command\n");
        return -1;
    }

    int tok = lex_current_token(lex);
    lex_next_token(lex);
    if (tok == PIPE) {
        target->next = new_command();
        int result = parse_line(target->next, lex);
        if (result == -1) {
            return -1;
        }
        return 0;
    } else if (tok == EOL) {
        return 0;
    }

    printf("Syntax error\n");
    return -1;
}

char *parse_filename(lexer *lex) {
    if (lex_current_token(lex) != ARG) {
        printf("Expected a filename\n");
        return NULL;
    }
    char *input = lex_get_string(lex);
    lex_next_token(lex);

    return input;
}

int parse_command(command *cmd, lexer *lex) {
    stringarr *args = new_stringarr();
    char *inputFile = NULL, *outputFile = NULL;
    while (!is_terminator(lex_current_token(lex))) {
        int token = lex_current_token(lex);
        if (token == REDIRECT_INPUT || token == REDIRECT_OUTPUT) {
            lex_next_token(lex);
            char *filename = parse_filename(lex);
            if (filename == NULL) {
                stringarr_free(args);
                return -1;
            }
            if (token == REDIRECT_INPUT)
                inputFile = filename;
            else outputFile = filename;
        } else if (token == ARG) {
            char *arg = lex_get_string(lex);
            stringarr_append(args, arg);
            lex_next_token(lex);
        }
    }
    stringarr_append(args, NULL);

    cmd->args = args;
    cmd->inputFile = inputFile;
    cmd->outputFile = outputFile;

    return 0;
}

int is_terminator(int token_type) {
    return token_type == PIPE || token_type == EOL;
}
