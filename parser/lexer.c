#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

lexer* new_lexer(stringarr *words){
    lexer *lex = malloc(sizeof(lexer));

    lex->current_word = 0;
    lex->words = words;
    return lex;
}

int lex_current_token(lexer *lex){
    if (lex->current_word == lex->words->count) return EOL;
    char *next = lex->words->values[lex->current_word];

    if ( strcmp(next, ">") == 0 ) return REDIRECT_OUTPUT;
    else if ( strcmp(next, "<") == 0 ) return REDIRECT_INPUT;
    else if ( strcmp(next, "|") == 0 ) return PIPE;
    return ARG;
}

void lex_next_token(lexer *lex){
    lex->current_word++;
}

char* lex_get_string(lexer *lex){
    return stringarr_get_copy(lex->words, lex->current_word);
}

