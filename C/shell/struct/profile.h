#ifndef _PROFILE_H
#define _PROFILE_H

struct profile
{
    char* name;
};

typedef struct profile Profile;

Profile* profile_init();

void profile_clean(Profile *);

#endif
