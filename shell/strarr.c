#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct strarr
{
    char **argv;
    int argc;
};

typedef struct strarr strarr;

strarr* strarr_init()
{
    strarr* tmp = (strarr*)malloc(sizeof(strarr));
    tmp -> argv = NULL;
    tmp -> argc = 0;
    return tmp;
}

void strarr_push(strarr *tmp,char* str)
{
    if (tmp->argv == NULL)
        tmp->argv = (char**)malloc(sizeof(char*));
    tmp->argv[tmp->argc] = (char*)malloc(sizeof(char)*(strlen(str)+1));
    strcpy(tmp->argv[tmp->argc],str);
    tmp->argc += 1;
}

void strarr_clear(strarr* tmp)
{
    int i;
    for(i = 0;i < tmp->argc;i++)
        free(tmp->argv[i]);
    free(tmp->argv);
    free(tmp);
}

int main()
{
    strarr *a = strarr_init();
    strarr_push(a,"hello");
    strarr_push(a,"world");
    int i;
    for(i=0;i<a->argc;i++)
        printf("%s\n",a->argv[i]);
    strarr_clear(a);
    return 0;
}
