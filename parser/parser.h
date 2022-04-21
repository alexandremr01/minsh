#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>

#include "../types/command.h"
#include "../types/stringarr.h"
#include "lexer.h"

// parse takes a string array and returns a linked list of commands
command* parse(stringarr *words);

#endif