#include "stringarr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

stringarr *new_stringarr(){
    stringarr *s = malloc(sizeof(stringarr*));
    s->count = 0;
    s->cap = 5;
    s->values = malloc(s->cap*sizeof(char*));
    return s;
}

void stringarr_append(stringarr *s, char *str){
    s->values[s->count] = str;
    if (s->count == s->cap - 1){
        s->cap = 2*s->cap;
        s->values = realloc(s->values, s->cap*sizeof(char*));
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

//    for (int i=0;i<parts->count;i++){
//        printf("String %d: %s\n", i, parts->values[i]);
//    }
    return parts;
}
