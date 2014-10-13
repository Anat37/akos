#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define OPEN_FILE_ERR -2
#define UNEXPECTED_ERR -100
#define ALLOCATE_MEM_ERR -1


int get_long_char(char ** string,FILE* fp )
{
    char *str=NULL;
    const int PAGE_SIZE = SHRT_MAX;
    size_t file_size = 0;
    char *tmp = (char*)malloc(PAGE_SIZE*sizeof(char));
    char *tmp1;

    str = (char*)malloc(sizeof(char));
    str[0] = '\0';
    
    if (fp == NULL)
    {
        free(tmp);
        return OPEN_FILE_ERR;
    }

    if ((tmp == NULL) || (str == NULL))
    {
        if (tmp != NULL)
            free(tmp);
        return ALLOCATE_MEM_ERR;
    }

    while (1)
    {
        tmp1 = fgets(tmp,PAGE_SIZE,fp);
        if (tmp1 == NULL)
        {
            free(tmp);
            *string=str;
            return 0;
        }

        file_size += strlen(tmp)+1;

        str = (char*)realloc(str,file_size*sizeof(char));
        if (str == NULL)
        {
            free(tmp);
            return ALLOCATE_MEM_ERR;
        }
        strcat(str,tmp);
    }

    return UNEXPECTED_ERR;
}

int main(int argc,char **argv)
{
    char *str=NULL;
    FILE *fp = fopen(argv[1],"r");
    
    while(get_long_char(&str,fp)==0)
    {
         printf("%s",str);
         if(str!=NULL)
         {
            free(str);
            str=NULL;
         }
    }

    fclose(fp);

    return 0;
}

