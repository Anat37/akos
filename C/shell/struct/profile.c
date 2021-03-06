/*
 * пользовательские данные
 */

#include <stdlib.h>
#include <string.h>

struct profile
{
    char* name;
};

typedef struct profile Profile;

Profile* profile_init()
{
    Profile* user = (Profile*)malloc(sizeof(Profile));
    user->name = (char*)malloc(sizeof(char)*256);
    strcpy(user->name,getenv("USER"));

    return user;
}

void profile_clean(Profile *user)
{
    free(user->name);
    free(user);
}
