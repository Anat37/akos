/*
 * структура программы и работа со структурой
 */

#include <stdlib.h>
#include <stdio.h>
#include "strarr.h"

struct program
{
    char *name;
    Strarr *args;
    char *input_file,*output_file;
    int output_type;
};

typedef struct program Program;

Program * program_init()
{
    Program *prog = (Program*)malloc(sizeof(Program));
    prog->name = NULL;
    prog->args = NULL;
    prog->input_file = NULL;
    prog->output_file = NULL;
    return prog;
}

void program_clean(Program *prog)
{
    strarr_clear(prog->args);
    if (prog->input_file!=NULL)
        free(prog->input_file);
    if (prog->output_file!=NULL)
        free(prog->output_file);
    free(prog);
}

void program_print(Program *prog)
{
    int i;
    
    if (prog->args->argc>0)
    {
        printf("name = %s\n",prog->args->argv[0]);
        if (prog->output_file != NULL)
            printf("output_file = %s with type = %i\n",prog->output_file,prog->output_type);
        if (prog->input_file != NULL)
            printf("input_file = %s\n",prog->input_file);
    }

    for(i=0;i < prog->args->argc; i++)
        printf("arg[%i] = %s\n",i,prog->args->argv[i]);
}
