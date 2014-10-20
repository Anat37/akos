#include <stdio.h>
#include <stdlib.h>

#define END_OF_FILE 1
#define MEMORY_ERR 2
#define FILE_ERR 3
#define DEFAULT_STRING_SIZE 1

void my_strcpy(int *to,int *from)
{
    while((*to++=*from++)!='\0')
        ;
}

int my_strcmp(int *first,int *second)
{
    for(;*first==*second;first++,second++)
        if (*first=='\0')
            return 0;
    return *first - *second;
}

int my_getc(FILE *in)
{
    int c;
    fread(&c,sizeof(char),1,in);
    return c;
}

void my_fprintf(int* string, size_t len, FILE* out)
{
    int i;
    char c;
    for(i=0;i<len;i++)
    {
        c = (char)string[i];
        fwrite(&c, sizeof(char), 1, out);
    }
}

int get_string(int** string, int* len, FILE *in)
{
    int c;
    int strsize = 1;
    int *str = (int*)malloc((strsize+1)*sizeof(int));
    int strpos = 1;

    if(in == NULL)
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
    
    while(1)
    {
        c = my_getc(in);
        if (feof(in))
        {
            free(str);
            return END_OF_FILE;
        }
        if (strpos == strsize)
        {
            strsize *= 2;
            str = (int*)realloc(str,(strsize+1)*sizeof(int));
            if (str == NULL)
            {
                return MEMORY_ERR;
            }
        }
        str[strpos-1] = c;
        strpos++;
        if (c == '\n')
        {
            str[strpos-1] = '\0';
            *string = str;
            *len = strpos-1;
            return 0;
        }
    }
    return 1; /* because we can't reach here*/
}

int main(int argv, char** argc)
{
    FILE *in;
    FILE *out;
    int *tmps = NULL;
    int *mins = NULL;
    short int *used = NULL;
    //int *times = NULL;
    int screen_width;

    int mini;
    int len;
    int filepos = 0;
    int filelen;

    if(argv<4)
    {
        printf("Need more arguments");
        return 0;
    }
    in = fopen(argc[1],"rb");
    out = fopen(argc[2],"wb");
    screen_width = atoi(argc[3]);
     
    while(!get_string(&tmps,&len,in))
    {
        filepos += 1;
    }
    

    used = (short int*)calloc(filepos,sizeof(short int));
    filelen = filepos;

    while (1)
    {
        fseek(in, 0, SEEK_SET);
        filepos = -1;
        
        do
        {
            if(get_string(&mins,&len,in))
                {
                    //print(times,screen_width);
                    //free(times);
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
        
        while(!get_string(&tmps,&len,in))
        {
            filepos++;
            if (!(used[filepos]) && (my_strcmp(mins,tmps) > 0))
            {
                mins = (int*)realloc(mins,(len+1)*sizeof(int));
                my_strcpy(mins,tmps);
                mini = filepos;
            }
        }
        used[mini] = 1;
        my_fprintf(mins,len,out);
       //analyze(mins,len,&times);
    }
    return 0; /*because we can't reach here*/
}
