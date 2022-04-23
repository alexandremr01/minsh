#ifndef STRING_ARR
#define STRING_ARR

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct stringarr {
    char **values;
    int count;
    int cap;
} stringarr;

stringarr *new_stringarr();

void stringarr_append(stringarr *s, char *str);

stringarr *split_string(char *str, char *delimiter);

void stringarr_free(stringarr *s);

char *stringarr_get_copy(stringarr *s, int i);

#endif