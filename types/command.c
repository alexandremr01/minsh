#include "command.h"
#include <stdlib.h>

command* append_command(command* current_command, stringarr *args){
    command *cmd = new_command(args);
    current_command->next = cmd;
    return cmd;
}

command* new_command(stringarr *str){
    command *new_command = malloc(sizeof(command));
    new_command->args = str;
    new_command->input = -1;
    new_command->output = -1;
    new_command->outputFile = NULL;
    new_command->inputFile = NULL;
    new_command->next = NULL;
    return new_command;
}