#include "command.h"

command* append_command(command* current_command, stringarr *args){
    command *cmd = new_command(args);
    current_command->next = cmd;
    return cmd;
}

command* new_command(){
    command *new_command = malloc(sizeof(command));
    new_command->args = NULL;
    new_command->input = -1;
    new_command->output = -1;
    new_command->outputFile = NULL;
    new_command->inputFile = NULL;
    new_command->next = NULL;
    return new_command;
}

void free_commands(command *cmd){
    command *p = cmd;
    while(p!=NULL){
        command *q = p;
        p = p->next;
        if (q->args != NULL)
            stringarr_free(q->args);
        if (q->inputFile != NULL)
            free(q->inputFile);
        if (q->outputFile != NULL)
            free(q->outputFile);
        free(q);
    }
}