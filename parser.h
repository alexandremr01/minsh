#ifndef PARSER_H
#define PARSER_H
#include "types/command.h"

// parse takes a string array and returns a linked list of commands
command* parse(stringarr *words);

#endif