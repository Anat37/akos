#ifndef _PROGRAM_H
#define _PROGRAM_H	

#include "strarr.h"

struct program
{
    char *name;
    Strarr *args;
    char *input_file,*output_file;
    int output_type;
};

typedef struct program Program;

Program * program_init();

void program_clean(Program *);

void program_print(Program *);

#endif
