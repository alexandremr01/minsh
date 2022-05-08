#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>

#include "../types/program.h"
#include "../types/stringarr.h"
#include "lexer.h"

int parse(program *head, stringarr *words);

#endif