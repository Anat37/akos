#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define END_OF_FILE 1
#define MEMORY_ERR 2
#define FILE_ERR 3
#define DEFAULT_STRING_SIZE 1

int my_getc(FILE *in)
{
    char c;
    if(!read(&c,sizeof(char),1,in))
    {
        /* ПАТАМУЧТО */
        return EOF;
    }
    return c;
}

int get_string(int** string, int *size, FILE *in)
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
            str[strpos-1] = '\0';
            *string = str;
            *size = strpos;
            return 0;
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
    }
    return 1; /* because we can't reach here*/
}

int main()
{
    FILE *in = fopen("a.out","rb");
    
    int c;
    int n = 0;
    
    while (1)
    {
        c = my_getc(in);
        if (c == '\0')
            break;
        n++;
    }

    printf("\n%i\n",n);
    fclose(in);
    return 0;
}
