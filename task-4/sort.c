#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define END_OF_FILE 1
#define MEMORY_ERR 2
#define FILE_ERR 3
#define DEFAULT_STRING_SIZE 1


void my_strcpy(char *to,char *from,int len)
{
    int i;
    for(i=0;i<len;i++)
    {
        *to++=*from++;
    }
}

int my_strcmp(char *f,char *s,int lenf,int lens)
{
    int i;
    for(i=0; (i<lenf)&&(i<lens); i++)
        if (f[i] != s[i])
        {
            return f[i]-s[i];
        }
    return lenf-lens;

}

int min(int* times)
{
    int i = 0;
    int min;
    while(times[i]==0)
        i++;
    min = times[i];

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
    if (times == NULL)
    {
        printf("There is 0 symbols, and 0 lines\n");
        return;
    }
    for(;i<256;i++)
    {
        if (times[i])
        {
            sum+=times[i];
            if (!isprint(i))
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
    printf("There is %i symbols, and %i lines\n",sum,times[10]);
}

int analyze(char* str,int len,int **times)
{
    int i;
    if(*times == NULL)
    {
        *times = (int*)calloc(256,sizeof(int));
        if (*times == NULL)
        {
            return MEMORY_ERR;
        }
    }
    
    for(i=0;i<len;i++)
    {
        (*times)[(unsigned char)str[i]]++;
    }
    return 0;
}

int my_getc(FILE *in)
{
    char c;
    if(!fread(&c,sizeof(char),1,in))
    {
        return EOF;
    }
    return c;
}

int get_string(char** string, FILE *in)
{
    int c;
    int strsize = DEFAULT_STRING_SIZE;
    char *str = (char*)malloc(strsize*sizeof(char));
    int strpos = 0;

    if(in == NULL)
    {
        return FILE_ERR;
    }
    if(str == NULL)
    {
        return MEMORY_ERR;
    }
    if(*string != NULL)
    {
        free(*string);
    }
    *string = NULL;
    
    while( fread(&c,sizeof(char),1,in) )
    {
        if (strpos == strsize)
        {
            strsize *= 2;
            str = (char*)realloc(str,strsize*sizeof(char));
            if (str == NULL)
            {
                return MEMORY_ERR;
            }
        }
        str[strpos] = c;
        strpos++;

        if(c == '\n')
        {
            break;
        }
    }
    *string = str;
    if (!strpos)
    {
        free(str);
        *string = NULL;
    }
    return strpos;
}

int main(int argv, char** argc)
{
    FILE *in;
    FILE *out;
    char *tmps = NULL;
    char *mins = NULL;
    short int *used = NULL;
    int *times = NULL;
    int screen_width;
    
    int len;
    int mini;
    int minlen;
    int filepos;

    if(argv<4)
    {
        printf("Need more arguments");
        return 0;
    }
    in = fopen(argc[1],"rb");
    out = fopen(argc[2],"wb");
    screen_width = atoi(argc[3]);
   
    filepos = 0;
    while( (len = get_string(&tmps,in)))
    {
        filepos += 1;
    }

    used = (short int*)calloc(filepos,sizeof(short int));

    while (1)
    {
        fseek(in, 0, SEEK_SET);
        filepos = -1;
        
        do
        {
            if(! (len = get_string(&mins,in)))
                {
                    print(times,screen_width);
                    free(times);
                    free(tmps);
                    free(mins);
                    free(used);
                    fclose(in);
                    fclose(out);
                    return 0;
                }
            filepos++;
        } while(used[filepos] == 1);
        mini = filepos;
        minlen = len;
        
        while( (len = get_string(&tmps,in)) )
        {
            filepos++;
            if (!(used[filepos]) && (my_strcmp(mins,tmps,minlen,len) > 0))
            {
                mins = (char*)realloc(mins,(len+1)*sizeof(char));
                my_strcpy(mins,tmps,len);
                mini = filepos;
                minlen = len;
            }
        }
        used[mini] = 1;
        fwrite(mins,sizeof(char),minlen,out);
        analyze(mins,minlen,&times);
    }
    return 1; /*because we can't reach here*/
}
