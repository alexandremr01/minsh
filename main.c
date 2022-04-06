#include <stdio.h>
#include "types/stringarr.h"
#include "types/command.h"
#include "parser.h"
#include "runner.h"
#include "interactive.h"

int main(){
    printf("MINi SHell 0.1.0 by Alexandre Maranhao\n\n");
    while(1){
        stringarr *command_line = prompt_command();

        command *commands = parse(command_line);
        
        if (validate(commands) != 0) continue;

        run_commands(commands);
    }
    return 0;
}