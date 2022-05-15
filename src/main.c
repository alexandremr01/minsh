#include <stdlib.h>
#include <stdio.h>

#include "types/stringarr.h"
#include "types/program.h"

#include "interactive/interactive.h"
#include "parser/parser.h"
#include "runner/runner.h"
#include "internal/internal.h"
#include "types/job.h"

void deallocate(stringarr *command_line, job *j) {
    stringarr_free(command_line);
//    free_jobs(j);
}

int main(){
    init_shell();

    job *jobs = malloc(sizeof(job));
    jobs->next = NULL;

    while(1) {
        stringarr *command_line = prompt_command();
        // in the case of an EOL
        if (command_line == NULL)
            break;

        if(is_internal_command(command_line)){
            execute_internal_command(command_line, jobs);
            continue;
        }

        // uses the array of strings to fill a linked list of programs
        job *j = parse(command_line);
        // syntax and semantic validation
        if (j == NULL || validate(j) != 0) {
//            deallocate(command_line, j);
            continue;
        }

        // execution
        execute_programs(jobs, j);

        // deallocate memory
//        deallocate(command_line, j);

        validate_running_programs(jobs);
    }

    return 0;
}
