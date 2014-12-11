#ifndef _PROFILE_H
#define _PROFILE_H

#include "dictionary.h"

struct profile
{
    char* name;
    Dict* dictionary;
};

typedef struct profile Profile;

Profile* profile_init();

void profile_clean(Profile *);

#endif
