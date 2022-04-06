#ifndef PARSER_H
#define PARSER_H
#include "types/command.h"

command* parse(stringarr *words);
command* parse_line(stringarr *words, int *current_word);
#endif