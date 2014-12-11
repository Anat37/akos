/*
 * структура для конвеера
 */

#include <stdlib.h>
#include "program.h"

struct job
{
    Program **program;
    int number_of_programs;
};

typedef struct job Job;

Job *job_init()
{
    Job *j = (Job*)malloc(sizeof(Job));
    j->program = NULL;
    j->number_of_programs = 0;
    return j;
}

void job_clean(Job *j)
{
    int i;
    
    for (i = 0;i < j->number_of_programs; i++)
        program_clean(j->program[i]);
    free(j);
}

void job_push(Job *j,Program *prog)
{
    j->number_of_programs += 1;
    j->program = (Program**)realloc(j->program,sizeof(Program*)*(j->number_of_programs));
    j->program[j->number_of_programs-1] = prog;
    return;
}

void job_print(Job *j)
{
    int i;
    for (i = 0;i<j->number_of_programs;i++)
        program_print(j->program[i]);
}
