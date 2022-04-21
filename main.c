#include <stdio.h>
#include <stdlib.h>
#include "types/stringarr.h"
#include "types/command.h"
#include "parser/parser.h"
#include "runner.h"
#include "interactive.h"
#include <signal.h>
#include <sys/types.h>

pid_t current_foreground_process = -1;

void signal_dispatcher (int signum) {
    if (current_foreground_process == -1)
        exit(0);
    kill(current_foreground_process, signum);
}

int main(){
    printf("MINi SHell 0.1.0 by Alexandre Maranhao\n\n");
    if (signal (SIGINT, signal_dispatcher) == SIG_IGN)
        signal (SIGINT, SIG_IGN);

    while(1){
        stringarr *command_line = prompt_command();
//      turns the array of strings into a linked list of commands
        command *commands = parse(command_line);
        stringarr_free(command_line);

        if (validate(commands) != 0) continue;

        run_commands(commands);

        free_commands(commands);
    }
    return 0;
}
