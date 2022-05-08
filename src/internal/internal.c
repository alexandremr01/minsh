#include "internal.h"

void internal_exit(stringarr *command_line) {
    stringarr_free(command_line);
    exit(0);
}

typedef void (*internal_executor)(stringarr *command_line); // function pointer

char *internal_commands[2] = {"exit", NULL};
internal_executor internal_commands_executors[2] = {internal_exit, NULL};

int get_internal_command_index(stringarr *command_line){
    int index = -1;
    if (command_line->count == 0)
        return -1;
    
    for (int i = 0; internal_commands[i] != NULL; i++){
        if (strcmp(command_line->values[0], internal_commands[i]) == 0) {
            return i;
        }
    }
    return index;
}

int is_internal_command(stringarr *command_line){
    return get_internal_command_index(command_line) != -1;
}

void execute_internal_command(stringarr *command_line){
    int index = get_internal_command_index(command_line);
    internal_executor executor = internal_commands_executors[index];
    executor(command_line);
    return;
}
