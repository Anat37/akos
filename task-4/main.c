#include <stdio.h>

int get_long_char(char ** string,char* file_name )
{
    FILE *fp = fopen(file_name,"r");
    if (fp == NULL)
    {
     return -2;
    }
    int PAGE_SIZE = SHRT_MAX;
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

int main(int argv, char** argc)
{
    int i = 0;
    FILE *in_file = fopen(argc[1],"r");
    FILE *out_file = fopen(argv[2],"w");
    /*добавить проверки*/


    return 0;

