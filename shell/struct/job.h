#ifndef _JOB_H
#define _JOB_H

#include "program.h"

struct job
{
    Program **program;
    int number_of_programs;
};

typedef struct job Job;

Job *job_init();

void job_clean(Job *);

void job_push(Job *, Program *);

void job_print(Job *);

#endif
