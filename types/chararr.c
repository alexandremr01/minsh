#include "chararr.h"
#include <stdlib.h>

chararr* newchararr() {
    chararr *s = malloc(sizeof(chararr));
    s->count = 0;
    s->cap = 256;
    s->str = malloc(s->cap*sizeof(char));
    return s;
}

void chararr_append(chararr *s, char c) {
    s->str[s->count] = c;
    if (s->count == s->cap - 1){
        s->cap = 2*s->cap;
        s->str = realloc(s->str, s->cap*sizeof(char));
    }
    s->count++;
}