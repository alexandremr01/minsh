#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>

#include "../types/command.h"
#include "../types/stringarr.h"
#include "lexer.h"

int parse(command *head, stringarr *words);

#endif