#ifndef CHAR_ARR
#define CHAR_ARR

typedef struct chararr {
    char *str;
    int cap, count;
} chararr;

chararr* newchararr();
void chararr_append(chararr *s, char c);

#endif