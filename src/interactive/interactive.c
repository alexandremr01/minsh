#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "interactive.h"

#define WELCOME_MESSAGE "MINi SHell 0.1.0 by Alexandre Maranhao\n\n"

pid_t current_foreground_process = -1;
struct termios shell_tmodes;

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
        switch(signum){
        case SIGINT:
            exit(0);
        case SIGTSTP:
        case SIGTTOU:
        default:
            return;
        }
    kill(current_foreground_process, signum);
    current_foreground_process = -1;
}

void init_shell() {
    printf(WELCOME_MESSAGE);
    using_history();
    signal(SIGINT, signal_dispatcher);
    signal(SIGTSTP, signal_dispatcher);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // save terminal modes
    tcgetattr(STDIN_FILENO, &shell_tmodes);

    int shell_pgid = getpid ();
    if (setpgid (shell_pgid, shell_pgid) < 0)
    {
        perror ("Couldn't put the shell in its own process group");
        exit (1);
    }
}
