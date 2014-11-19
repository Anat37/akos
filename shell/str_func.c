#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

Dict *d;

void str_cut(char* str,size_t pos,size_t len)
{
    size_t i;
    for(i=pos;str[i+len];i++)
        str[i] = str[i+len];
    str[i] = '\0';
}

void insert_vals(char **str)
{
    size_t pos;
    size_t i;
    size_t j;
    char val = 0;
    char *value;
    char *key;
    char *tmp;

    i = 0;
    do
    {
        if ( (*str)[i]=='$' )
        {
            pos = i;
            val = 1;
        }
        if (( ((*str)[i] == ' ')||( (*str)[i] == '\0') ) && val)
        {
            key = (char*)malloc(sizeof(char)*(i-pos+1));
            for(j=pos;j<i;j++)
                key[j-pos] = (*str)[j];
            key[j-pos] = '\0';
            
            value = dict_get(d,key);
            printf("key = %s value = %s\n",key,value);
            
            *str = (char*)realloc(*str,sizeof(char)*(strlen(*str)+ strlen(value) - strlen(key)));
            str_cut(*str,pos+1,i-pos-1);
            (*str)[pos] = '\0';
            
            tmp = (char*)malloc(sizeof(char)*(strlen(*str)-pos));
            for(j = pos+1;(*str)[j];j++)
                tmp[j-pos-1] = (*str)[j];
            tmp[j-pos-1] = '\0';

            strcat(*str,value);
            strcat(*str,tmp);
            
            i = pos;
            val = 0;
        }
    }while((*str)[i++]);
}

int main()
{
    d = dict_init();
    dict_append(d,"$USER","kozlovnikita");
    
    char* str = (char*)malloc(sizeof(char)*256);
    scanf("%[^\n]s",str);
    getchar();
    
    insert_vals(&str);
    printf("%s\n",str);
    return 0;
}
