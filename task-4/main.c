#include <stdio.h>
#include <stdlib.h>

#define END_OF_FILE 1
#define MEMORY_ERR 2
#define FILE_ERR 3
#define DEFAULT_STRING_SIZE 1

void my_strcpy(char *to,char *from)
{
    while((*to++=*from++)!='\0')
        ;
}

int my_strcmp(char *first,char *second)
{
    for(;*first==*second;first++,second++)
        if (*first=='\0')
            return 0;
    return *first - *second;
}

int my_strlen(char *string)
{
    int c = 0;
    while (*string++)
    {
        c++;
    }
    return c+1;
}

int min(int* times)
{
    int i = 0;
    int min;
    for(;(times[i]==0) && (i<256); i++)
        ;
    if (i<256)
    {
        min = times[i];
    }else
    {
        min = 0;
    }
    for(;i<256;i++)
    {
        if ((times[i]<min)&&(times[i]>0))
        {
            min = times[i];
        
        }
    }
    return min;
}

void print(int *times, int screen_width)
{
    int i = 0;
    int j = 0;
    int counter = 0;
    int sum = 0;
    for(;i<256;i++)
    {
        if (times[i])
        {
            sum+=times[i];
            if (i<33)
            {
                printf("%i ",i);
                
            }else
            {
                printf("'%c' ",i);
            }
            counter = times[i]/min(times);
            for(j = 0;(j<counter)&&(j<screen_width);j++)
            {
                printf("*");
            }
            printf("\n");
        }
    }
    printf("There is %i symbols, and %i lines",sum,times[10]);

}

int analyze(char* str,int **times)
{
    if(*times == NULL)
    {
        *times = (int*)calloc(256,sizeof(int));
        if (*times == NULL)
        {
            return MEMORY_ERR;
        }
    }
    for(;*str!='\0';str++)
    {
        (*times)[(unsigned int)*str]++;
    }
    return 0;
}

int get_string(char** string, FILE *fp)
{
    int c;
    int strsize = 1;
    char *str = (char*)malloc((strsize+1)*sizeof(char));
    int strpos = 1;

    if(fp == NULL)
    {
        return FILE_ERR;
    }
    if (str == NULL)
    {
        return MEMORY_ERR;
    }
    if(*string != NULL)
    {
        free(*string);
    }
    *string = NULL;
    
    while( (c = getc(fp)) != EOF )
    {
        if (strpos == strsize)
        {
            strsize *= 2;
            str = (char*)realloc(str,(strsize+1)*sizeof(char));
            if (str == NULL)
            {
                return MEMORY_ERR;
            }
        }
        str[strpos-1] = (char)c;
        strpos++;
        if (c == '\n')
        {
            str[strpos-1] = '\0';
            *string = str;
            return 0;
        }
    }
    free(str);
    return END_OF_FILE;
}

int main(int argv, char** argc)
{
    FILE *in_file = fopen(argc[1],"r");
    FILE *out_file = fopen(argc[2],"w");
    char *tmps = NULL;
    char *mins = NULL;
    short int *used = NULL;
    int *times = NULL;
    int screen_width = atoi(argc[3]);

    int mini;
    int filepos = 0;
    int filelen;
     
    while(!get_string(&tmps,in_file))
    {
        filepos += 1;
    }
    
    used = (short int*)calloc(filepos,sizeof(short int));
    filelen = filepos;

    while (1)
    {
        fseek(in_file, 0, SEEK_SET);
        filepos = -1;
        
        do
        {
            if(get_string(&mins,in_file))
                {
                    print(times,screen_width);
                    free(times);
                    free(tmps);
                    free(mins);
                    free(used);
                    fclose(in_file);
                    fclose(out_file);
                    return 0;
                }
            filepos++;
        } while(used[filepos] == 1);
        mini = filepos;
        
        while(!get_string(&tmps,in_file))
        {
            filepos++;
            if (!(used[filepos]) && (my_strcmp(mins,tmps) > 0))
            {
                mins = (char*)realloc(mins,my_strlen(tmps)*sizeof(char));
                my_strcpy(mins,tmps);
                mini = filepos;
            }
        }
        used[mini] = 1;
        fprintf(out_file,"%s", mins);
        analyze(mins,&times);
    }

    return 1; /*because we can't reach here*/
}
