#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void close_files(int num,...)
{
    int i;
    va_list  args;
    va_start(args,num);
    for(i=0;i<num;i++)
    {
        fclose(va_arg(args, FILE*));
    }
    va_end(args);
}

char is_there(char* arr,int arrlen)
{
    int i;
    for(i=0;i<arrlen;i++)
        if (arr[i]==0)
            return 1;
    return 0;
}

#define stringlen 256

int main(int argv, char** argc)
{
    FILE *in_file = fopen(argc[1],"r");
    FILE *out_file = fopen(argc[2],"w");
    char *tmps = (char*)malloc(stringlen*sizeof(char));
    int mini = 0;
    int filepos = 0;
    char *mins = (char*)malloc(stringlen*sizeof(char));
    fgets(mins,stringlen,in_file);
   
    while (fgets(tmps,stringlen,in_file))
    {
        filepos += 1;
        if (strcmp(mins,tmps) > 0)
        {
            strcpy(mins,tmps);
            mini = filelen;
        }
    }
    printf("len = %i\nstring = %spos = %i\n",filelen+1,mins,mini);
    
    char *used = (char*)calloc(filepos,sizeof(char));
    used[mini] = 1;
    fprintf(out_file,"%s",mins);
    
    while (is_there(used,filelen))
    {
        filepos = 0;
        fseek(in_file, 0, SEEK_SET);
        while
    }

    free(mins);
    free(tmps);
    close_files(2,in_file,out_file);
    return 0;
}
