#ifndef STRING_ARR
#define STRING_ARR

typedef struct stringarr{
    char **argv;
    int count;
    int cap;
} stringarr;

stringarr *new_stringarr();
void stringarr_append(stringarr *s, char *str);

#endif