#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stringarr.h"

stringarr *new_stringarr(){
    stringarr *s = malloc(sizeof(stringarr));
    s->count = 0;
    s->cap = 5;
    s->values = malloc(s->cap*sizeof(char*));
    return s;
}

void stringarr_append(stringarr *s, char *str){
    s->values[s->count] = str;
    s->count++;
    if (s->count == s->cap - 1){
        s->cap = 2*s->cap;
        s->values = realloc(s->values, s->cap*sizeof(char*));
    }
}

stringarr *split_string(char *str, char *delimiter){
    stringarr *parts = new_stringarr();
    char *token = strtok(str, delimiter);
    while (token != NULL){
        if (strcmp(token, "") == 0) continue;
        char *cpy = malloc((strlen(token)+1) * sizeof(char));
        strcpy(cpy, token);
        stringarr_append(parts, cpy);
        token = strtok(NULL, delimiter);
    }
    return parts;
}

void stringarr_free(stringarr *s){
    for(int i = 0; i < s->count; i++)
        if(s->values[i]!=NULL) free(s->values[i]);
    free(s->values);
    free(s);
}

char* stringarr_get_copy(stringarr *s, int i){
    if(i >= s->count) return NULL;
    char *cpy = malloc((strlen(s->values[i])+1) * sizeof(char));
    strcpy(cpy, s->values[i]);
    return cpy;
}

