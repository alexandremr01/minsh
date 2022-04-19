#include "interactive.h"
#include "types/chararr.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

stringarr *prompt_command(){
    using_history();
    char *line = readline("> ");
    if (line == NULL) exit(0);
    add_history (line);
    return split_string(line, " ");
}