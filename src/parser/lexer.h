#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <string.h>

#include "../types/program.h"

#define REDIRECT_OUTPUT 1
#define REDIRECT_INPUT 2
#define PIPE 3
#define EOL 4
#define ARG 5

typedef struct lexer {
    int current_word;
    stringarr *words;
} lexer;

lexer *new_lexer(stringarr *words);

int lex_current_token(lexer *lex);

void lex_next_token(lexer *lex);

char *lex_get_string(lexer *lex);

#endif
