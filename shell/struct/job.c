/*
 * структура для конвеера
 */

#include <stdlib.h>
#include <sys/types.h>
#include "program.h"

struct job
{
    int background;
    int done;
    Program **program;
    int number_of_programs;
};

typedef struct job Job;

Job *job_init()
{
    Job *j = (Job*)malloc(sizeof(Job));
    j->program = NULL;
    j->number_of_programs = 0;
    j->background = 0;
    j->done = 0;
    return j;
}

void job_turn_off(Job *j,pid_t pid)
{
    int i;
    int status = 1;
    for (i = 0;i < j->number_of_programs;i++)
    {
        if (j->program[i]->pid == pid)
        {
            j->program[i]->status = 0;
        }
        
        if (j->program[i]->status == 1)
            status = 0;
    }

    if (i == j->number_of_programs)
        j->done = status;

    return;
}

void job_clean(Job *j)
{
    int i;
    for (i = 0;i < j->number_of_programs; i++)
        program_clean(j->program[i]);
    free(j->program);
    free(j);
    return;
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
    return;
}
