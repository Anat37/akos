#ifndef _JOB_H
#define _JOB_H

#include <sys/types.h>
#include "program.h"

struct job
{
    int backgroung;
    int done;
    Program **program;
    int number_of_programs;
};

typedef struct job Job;

Job *job_init();

void job_turn_off(Job *,pid_t);

void job_clean(Job *);

void job_push(Job *, Program *);

void job_print(Job *);

#endif
