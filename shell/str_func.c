#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"


void str_cut(char* str,size_t pos,size_t len)
{
    size_t i;
    for(i=pos;str[i+len];i++)
        str[i] = str[i+len];
    str[i] = '\0';
}

void insert_vals(char **str,Dict *d)
{
    size_t pos;
    size_t i;
    size_t j;
    char val = 0;
    char *value;
    char *key;
    char *tmp;
    
    if(*str==NULL)
        return;
    i = 0;
    while(1)
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
            *str = (char*)realloc(*str,sizeof(char)*(strlen(*str) + strlen(value) - strlen(key)+1));
            str_cut(*str,pos+1,i-pos-1);
            
            tmp = (char*)malloc(sizeof(char)*(strlen(*str)-pos));
            strcpy(tmp,&(*str)[pos+1]);
            
            (*str)[pos] = '\0';
            strcat(*str,value);
            strcat(*str,tmp);
            
            free(tmp);
            free(key);
            i = pos;
            val = 0;
        }
        if((*str)[i] == '\0')
            break;
        i++;
    }
}

int main()
{
    Dict *d = dict_init();
    dict_append(d,"$USER","kozlovnikita");
    
    char* str = (char*)malloc(sizeof(char)*256);
    scanf("%[^\n]s",str);
    getchar();
    
    insert_vals(&str,d);
    printf("%s\n",str);
    
    free(str);
    dict_clear(&d);
    return 0;
}
