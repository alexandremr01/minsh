#include "program.h"

program *new_program() {
    program *new_program = malloc(sizeof(program));
    new_program->args = NULL;
    new_program->input = -1;
    new_program->output = -1;
    new_program->outputFile = NULL;
    new_program->inputFile = NULL;
    new_program->next = NULL;
    return new_program;
}

void free_programs(program *prg) {
    program *p = prg;
    while (p != NULL) {
        program *q = p;
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
