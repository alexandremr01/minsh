#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>

#include "../types/program.h"
#include "../types/stringarr.h"
#include "lexer.h"
#include "../types/job.h"

job* parse(stringarr *words);

#endif