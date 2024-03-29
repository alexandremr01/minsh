#include "parser.h"

int parse_line(program *target, lexer *lex);

int is_terminator(int token_type);

int parse_program(program *prg, lexer *lex);

job *parse(stringarr *words) {
    program *head = new_program();
    job *j = new_job(head, 1);
    j->command_line = words;
    if (words->count == 0)
        return j;

    lexer *lex = new_lexer(words);
    head->next = new_program();
    int result = parse_line(head->next, lex);
    if (lex_current_token(lex) == COMMERCIAL_AND) {
        lex_next_token(lex);
        j->foreground = 0;
        if (lex_current_token(lex) != EOL) {
            printf("Syntax error\n");
            return NULL;
        }
    }

    free(lex);

    if (result != 0)
        return NULL;
    return j;
}

int parse_line(program *target, lexer *lex) {
    int result = parse_program(target, lex);
    if (result == -1)
        return -1;
    if (target->args->count == 1) {
        printf("Error: empty command\n");
        return -1;
    }

    int tok = lex_current_token(lex);
    if (tok == PIPE) {
        lex_next_token(lex);
        target->next = new_program();
        int result = parse_line(target->next, lex);
        if (result == -1) {
            return -1;
        }
        return 0;
    } else if (tok == EOL || tok == COMMERCIAL_AND) {
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

int parse_program(program *prg, lexer *lex) {
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

    prg->args = args;
    prg->inputFile = inputFile;
    prg->outputFile = outputFile;

    return 0;
}

int is_terminator(int token_type) {
    return token_type == PIPE || token_type == EOL || token_type == COMMERCIAL_AND;
}
