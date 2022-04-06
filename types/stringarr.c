#include "stringarr.h"
#include <stdlib.h>
#include <string.h>

stringarr *new_stringarr(){
    stringarr *s = malloc(sizeof(stringarr*));
    s->count = 0;
    s->cap = 5;
    s->argv = malloc(s->cap*sizeof(char*));
    return s;
}

void stringarr_append(stringarr *s, char *str){
    s->argv[s->count] = str;
    if (s->count == s->cap - 1){
        s->cap = 2*s->cap;
        s->argv = realloc(s->argv, s->cap*sizeof(char*));
    }
    s->count++;
}

stringarr *split_string(char *str, char *delimiter){
    stringarr *parts = new_stringarr();
    char *token;
    while ((token = strsep(&str, delimiter)) != NULL){
        if (strcmp(token, "") == 0) continue;
        stringarr_append(parts, token);
    }
    stringarr_append(parts, "\0");
    return parts;
}
