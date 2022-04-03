#ifndef PARSER_H
#define PARSER_H
#include "job.h"

job* parse(stringarr *words);
job* parse_line(stringarr *words, int *current_word);
#endif