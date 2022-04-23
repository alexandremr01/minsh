#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "interactive.h"

#define WELCOME_MESSAGE "MINi SHell 0.1.0 by Alexandre Maranhao\n\n"

pid_t current_foreground_process = -1;

stringarr *prompt_command() {
    char *line = readline("> ");
    if (line == NULL)
        return NULL;
    if (strcmp(line, "") != 0)
        add_history(line);
    stringarr *words = split_string(line, " ");
    free(line);
    return words;
}

// signal_dispatcher sends a signal to the child foreground process, if it exists
void signal_dispatcher(int signum) {
    if (current_foreground_process == -1)
        exit(0);
    kill(current_foreground_process, signum);
}

void init_shell() {
    printf(WELCOME_MESSAGE);
    using_history();
    if (signal(SIGINT, signal_dispatcher) == SIG_IGN)
        signal(SIGINT, SIG_IGN);
}
