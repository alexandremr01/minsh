#include "stringarr.h"
#include <stdlib.h>

stringarr *new_stringarr(){
    stringarr *s = malloc(sizeof(stringarr*));
    s->count = 0;
    s->cap = 5;
    s->argv = malloc(s->cap*sizeof(char*));
}

void stringarr_append(stringarr *s, char *str){
    s->argv[s->count] = str;
    if (s->count == s->cap - 1){
        s->cap = 2*s->cap;
        s->argv = realloc(s->argv, s->cap*sizeof(char*));
    }
    s->count++;
}
