#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/*function read much as it can, and change first parametr
return 0 if all is ok 
return -1 if error with allocating memory
retrun -2 if error with opening file*/

int get_long_char(char ** string,char* file_name )
{
    FILE *fp = fopen(file_name,"r");
    if (fp == NULL)
    {
        return -2;
    }
    const int PAGE_SIZE = SHRT_MAX;
    unsigned long long int file_size = 0;
    *string = NULL; 
    char *tmp = (char*)malloc(PAGE_SIZE);
    if (tmp == NULL)
    {
        fclose(fp);
        return -1;
    }

    while (1)
    {
        tmp = fgets(tmp,PAGE_SIZE,fp);
        if (tmp == NULL)
        {
            fclose(fp);
            return 0;
        }
        file_size += PAGE_SIZE;
        *string = (char*)realloc(*string,file_size*sizeof(char));
        if (*string == NULL)
        {
            fclose(fp);
            return -1; 
        }
        strcat(*string,tmp);
    }
    fclose(fp);
    return 0;
}

int main()
{
    char **str = NULL;
    get_long_char(str,"small_input_data.txt");
    printf("%s\n",*str);
    return 0;
}
