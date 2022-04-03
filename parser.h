#ifndef PARSER_H
#define PARSER_H
#include "job.h"

job* parse(char **words);
job* parse_line(char **words, int *current_word);
#endif