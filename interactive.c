#include "interactive.h"
#include "types/chararr.h"
#include <readline/readline.h>
#include <readline/history.h>

stringarr *prompt_command(){
    using_history();
    char *line = readline("> ");
    add_history (line);
    return split_string(line, " ");
}