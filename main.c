#include <stdlib.h>

#include "types/stringarr.h"
#include "types/command.h"

#include "interactive.h"
#include "parser/parser.h"
#include "runner.h"

void deallocate(stringarr *command_line, command *commands_head) {
    stringarr_free(command_line);
    free_commands(commands_head);
}

int main(){
    init_shell();

    while(1) {
        stringarr *command_line = prompt_command();
        // in the case of an EOL
        if (command_line == NULL)
            break;

        command *commands_head = new_command();
        // uses the array of strings to fill a linked list of commands
        int result = parse(commands_head, command_line);
        // syntax and semantic validation
        if (result != 0 || validate(commands_head) != 0) {
            deallocate(command_line, commands_head);
            continue;
        }

        // execution
        execute_commands(commands_head);

        // deallocate memory
        deallocate(command_line, commands_head);
    }

    return 0;
}
