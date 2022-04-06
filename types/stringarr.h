#ifndef STRING_ARR
#define STRING_ARR

typedef struct stringarr{
    char **values;
    int count;
    int cap;
} stringarr;

stringarr *new_stringarr();
void stringarr_append(stringarr *s, char *str);
stringarr *split_string(char *str, char *delimiter);

#endif