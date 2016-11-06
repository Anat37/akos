#ifndef _PROGRAM_H
#define _PROGRAM_H	

#include <sys/types.h>
#include "strarr.h"

#define ON 1;
#define OFF 0;

struct program
{
    char *name;
    pid_t pid;
    int status;
    Strarr *args;
    char *input_file,*output_file;
    int output_type;
};

typedef struct program Program;

Program * program_init();

void program_clean(Program *);

void program_print(Program *);

#endif
