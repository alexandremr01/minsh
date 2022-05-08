#include <stdlib.h>
#include <stdio.h>

#include "types/stringarr.h"
#include "types/program.h"

#include "interactive/interactive.h"
#include "parser/parser.h"
#include "runner/runner.h"
#include "internal/internal.h"

void deallocate(stringarr *command_line, program *programs_head) {
    stringarr_free(command_line);
    free_programs(programs_head);
}

int main(){
    init_shell();

    while(1) {
        stringarr *command_line = prompt_command();
        // in the case of an EOL
        if (command_line == NULL)
            break;

        if(is_internal_command(command_line)){
            execute_internal_command(command_line);
            continue;
        }

        program *programs_head = new_program();
        // uses the array of strings to fill a linked list of programs
        int result = parse(programs_head, command_line);
        // syntax and semantic validation
        if (result != 0 || validate(programs_head) != 0) {
            deallocate(command_line, programs_head);
            continue;
        }

        // execution
        execute_programs(programs_head);

        // deallocate memory
        deallocate(command_line, programs_head);
    }

    return 0;
}
