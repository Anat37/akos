#ifndef STDLIB
    #include <stdlib.h>
    #define STDLIB
#endif

#ifndef STRING
    #include <string.h>
    #define STRING
#endif

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
    tmp->argv = (char**)realloc(tmp->argv,sizeof(char*)*(tmp->argc + 1));
    if (str==NULL)
        tmp->argv[tmp->argc] = NULL;
    else{
        tmp->argv[tmp->argc] = (char*)malloc(sizeof(char)*(strlen(str)+1));
        strcpy(tmp->argv[tmp->argc],str);
    }
    tmp->argc += 1;
}

strarr* strarr_slice(strarr *tmp,int start,int end)
{
    strarr* result = strarr_init();
    int i;
    for(i = start;i<end;i++)
        strarr_push(result,tmp->argv[i]);
    return result;
}

void strarr_clear(strarr* tmp)
{
    if (tmp==NULL)
        return;
    int i;
    for(i = 0;i < tmp->argc;i++)
        free(tmp->argv[i]);
    free(tmp->argv);
    free(tmp);
}
