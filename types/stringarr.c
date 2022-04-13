#include "stringarr.h"
#include "chararr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *copy(char* src);

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
        s->values = realloc(s->values, s->cap*sizeof(char));
    }
}

stringarr *split_string(char *str, char *delimiter){
    stringarr *parts = new_stringarr();
    char *token;
    while ((token = strsep(&str, delimiter)) != NULL){
        if (strcmp(token, "") == 0) continue;
        stringarr_append(parts, copy(token));
    }
    return parts;
}

void stringarr_free(stringarr *s){
    for(int i = 0; i < s->count; i++)
        if(s->values[i]!=NULL) free(s->values[i]);
    free(s->values);
    free(s);
}

char* copy(char* src) {
    chararr *ca = newchararr();
    for(unsigned int j=0; j <= strlen(src); j++) {
        chararr_append(ca, src[j]);
    }
    char *result = ca->str;
    free(ca);
    return result;
}

char* stringarr_get_copy(stringarr *s, int i){
    if(i >= s->count) return NULL;
    return copy(s->values[i]);
}

