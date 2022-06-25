#include <stdlib.h>

#include "types/stringarr.h"
#include "types/program.h"

#include "interactive/interactive.h"
#include "parser/parser.h"
#include "system/executor.h"
#include "internal/internal.h"
#include "types/job.h"

int main() {
    init_shell();

    job *jobs = new_job_head();

    while (1) {
        check_running_programs(jobs);

        stringarr *command_line = prompt_command();
        // in the case of an EOL
        if (command_line == NULL)
            break;

        if (is_internal_command(command_line)) {
            int stop = execute_internal_command(command_line, jobs);
            stringarr_free(command_line);
            if (stop) break;
            else continue;
        }

        // uses the array of strings to fill a linked list of programs
        job *j = parse(command_line);
        // syntax and semantic validation
        if (j == NULL || validate(j) != 0) {
            continue;
        }

        // execution
        execute_job(jobs, j);

    }

    // deallocate memory
    for (job *j = jobs; j;) {
        job *p = j;
        j = j->next;
        free_jobs(p);
    }

    return 0;
}
