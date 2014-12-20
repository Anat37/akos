#ifndef _STRARR_H
#define _STRARR_H

struct strarr
{
    char **argv;
    int argc;
};

typedef struct strarr Strarr;

Strarr* strarr_init();

void strarr_push(Strarr*,char* );

Strarr* strarr_slice(Strarr*,int,int);

char* strarr_pop(Strarr*,int);

void strarr_clear(Strarr*);

#endif
