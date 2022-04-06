#include <stdio.h>
#include "types/stringarr.h"
#include "types/job.h"
#include "parser.h"
#include "runner.h"
#include "interactive.h"

int main(){
    printf("MINi SHell 0.1.0 by Alexandre Maranhao\n\n");
    while(1){
        stringarr *command_line = prompt_command();

        job *jobs_head = parse(command_line);
        if (jobs_head == NULL) {
            continue;
        }

        if (validate(jobs_head) != 0) continue;

        run_jobs(jobs_head);
    }
    return 0;
}