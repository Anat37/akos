#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*void get_string(char **str)
{
    *str = (char*)malloc(10);
    strcpy(*str,"hello");
    *str = (char*)realloc(*str,15);
    return;
}
*/
int main()
{
    /*
    FILE *fp = fopen("small_input_data.txt","r");
    char *str = (char*)malloc(20);
    fgets(str,20,fp);
    str = realloc(str, 30);
    strcat(str," shit");

    printf("%s\n", str);

    free(str);
    fclose(fp);*/
    /*char **str = (char**)malloc(sizeof(char*));
    get_string(str);
    printf("%s",*str);
    free(*str);
    free(str);*/
    char *str = (char*)malloc( (strlen("hello")+2) * sizeof(char));
    strcpy(str,"hello");
    printf("%s",str);
    free(str);

    return 0;
}
