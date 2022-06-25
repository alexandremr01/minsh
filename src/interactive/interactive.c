#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "interactive.h"

#define WELCOME_MESSAGE "MINi SHell 0.2.0 by Alexandre Maranhao\n\n"

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

void init_shell() {
    printf(WELCOME_MESSAGE);
    using_history();
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    // save terminal modes
    tcgetattr(STDIN_FILENO, &shell_tmodes);

    int shell_pgid = getpid();
    if (setpgid(shell_pgid, shell_pgid) < 0) {
        perror("Couldn't put the shell in its own process group");
        exit(1);
    }
}

void display_jobs(job *jobs) {
    printf("Current jobs\n");
    int k = 0;
    for (job *j = jobs->next; j; j = j->next, k++) {
        printf("[%d] ", k);
        switch (j->status) {
            case RUNNING:
                printf("Running: ");
                break;
            case STOPPED:
                printf("Stopped: ");
                break;
            case FINISHED:
                printf("Finished: ");
                break;
        }
        for (int i = 0; i < j->command_line->count; i++)
            printf("%s ", j->command_line->values[i]);
        printf("\n");
    }
}

void notify_ended(job *j) {
    printf("Job ");
    for (int i = 0; i < j->command_line->count; i++)
        printf("%s ", j->command_line->values[i]);
    printf("ended \n");
}
