/*
 * пользовательские данные
 */

#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

struct profile
{
    char* name;
    Dict* dictionary;
};

typedef struct profile Profile;

Profile* profile_init()
{
    Profile* user = (Profile*)malloc(sizeof(Profile));
    user->name = (char*)malloc(sizeof(char)*256);
    strcpy(user->name,getenv("USER"));
    user->dictionary = dict_init();
    dict_append(user->dictionary,"$USER",user->name);
    return user;
}

void profile_clean(Profile *user)
{
    dict_clear(&user->dictionary);
    free(user->name);
    free(user);
}
