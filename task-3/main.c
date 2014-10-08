#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/*
* function read much as it can, and change first parametr
* return 0 if all is ok 
* return -1 if error with allocating memory
* retrun -2 if error with opening file
*/

int get_long_char(char ** string,char* file_name )
{
    FILE *fp = fopen(file_name,"r");
    if (fp == NULL)
    {
        return -2;
    }
    const int PAGE_SIZE = SHRT_MAX;
    unsigned long long int file_size = 0;
    *string = (char*)malloc(sizeof(char));
    *string[0] = '\0';
    char *tmp = (char*)malloc(PAGE_SIZE*sizeof(char));
    char *tmp1;
    if (tmp == NULL)
    {
        free(tmp);
        fclose(fp);
        return -1;
    }
    while (1)
    {
        tmp1 = fgets(tmp,PAGE_SIZE,fp);
        if (tmp1 == NULL)
        {
            free(tmp);
            fclose(fp);
            return 0;
        }
        file_size += strlen(tmp)+1;
        *string = (char*)realloc(*string,file_size*sizeof(char));
        if (*string == NULL)
        {
            free(tmp);
            fclose(fp);
            return -1; 
        }
        strcat(*string,tmp);
    }
}

int main()
{
    char **str = (char**)malloc(sizeof(char*));
    get_long_char(str,"input_data.txt");
    printf("%s",*str);
    free(*str);
    free(str);
    return 0;
}
