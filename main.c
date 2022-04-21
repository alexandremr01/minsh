#include <stdlib.h>

#include "types/stringarr.h"
#include "types/command.h"

#include "interactive.h"
#include "parser/parser.h"
#include "runner.h"

int main(){
    init_shell();

    while(1) {
        stringarr *command_line = prompt_command();
        // in the case of an EOL
        if (command_line == NULL)
            break;

        // turns the array of strings into a linked list of commands
        command *commands = parse(command_line);
        // semantic validation
        if (validate(commands) != 0) {
            free_commands(commands);
            continue;
        }

        // execution
        execute_commands(commands);

        // deallocate memory
        stringarr_free(command_line);
        free_commands(commands);
    }
    
    return 0;
}
