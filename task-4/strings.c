#include <stdio.h>
#include <stdlib.h>

void my_strcpy(char *to, char* from)
{
    while((*to++=*from++)!='\0')
        ;
}

int my_strcmp(char* first, char *second)
{
    for(;*first==*second;first++,second++)
        if (*first=='\0')
            return 0;
    return *first-*second;
}

int my_strlen(char *string)
{
    int c = 0;
    while(*string++)
    {
        c++;
    }
    return c+1;
}

int main()
{
    char *to,*from;
    to = NULL;
    from = (char*)malloc(3*sizeof(char));
    from[0]='a';
    from[1]='b';
    from[2]='\0';
    to = (char*)realloc(to,my_strlen(from)*sizeof(char));
    my_strcpy(to,from);
    printf("%i %s\n",my_strlen(to),to);

    return 0;
}
