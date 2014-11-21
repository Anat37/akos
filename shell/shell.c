#ifndef STDIO
    #include <stdio.h>
    #define STDIO
#endif

#ifndef SETJMP
    #include <setjmp.h>
    #define SETJMP
#endif

#ifndef STRING
    #include <string.h>
    #define STRING
#endif

#ifndef STDLIB
    #include <stdlib.h>
    #define STDLIB
#endif

#include "dictionary.h"

/* 
 * описание конструкции try catch
 */

static jmp_buf err;

#define TRY \
    { \
        switch(setjmp(err)) \
        { \
            case 0:

#define CATCH(x) \
                break; \
            case x:

#define ENDTRY \
        }\
    }

#define THROW(x) longjmp(err,x);

/*
 * описание исключений
 */

#define MEMORY_ERR 1
#define FILE_ERR 2
#define QUOTES_ERR 3
#define MEMORY_ERR_TEXT "Some error with memory"
#define FILE_ERR_TEXT "Some error with file access"
#define QUOTES_ERR_TEXT "Some error with quotes"
/*
 * считывает команду произвольного размера
 */ 

char* read_long_line(FILE* infp)
{
    int max_size = 1,pos = 0;
    char *str = (char*)malloc(sizeof(char)*max_size);
    char tmp;
    char single_string = 0;
    char double_string = 0;
    char do_nothing = 0;
    char dont_read = 0;
    
    if (infp == NULL)
    {
        free(str);
        THROW(FILE_ERR)
    }

    if (str == NULL)
        THROW(MEMORY_ERR)
    
    if (feof(infp))
    {
        free(str);
        return NULL;
    }

    while(fread(&tmp,sizeof(char),1,infp))
    {
        if(pos==max_size)
        {
            max_size<<=1;
            str = (char*)realloc(str,sizeof(char)*max_size);
            if (str == NULL)
                THROW(MEMORY_ERR)
        }

        if ( (tmp=='\'') && (!double_string) && !do_nothing )
            single_string = (single_string)?0:1;

        if ( (tmp=='\"') && (!single_string) && !do_nothing )
            double_string = (double_string)?0:1;

        if ( ( tmp=='\n' ) 
                &&( !( single_string || double_string ) )&&(!do_nothing) )
            break;

        if ((tmp =='\n')&&(single_string || double_string))
            printf(" > ");
        
        if ((tmp == '#')&&(!do_nothing))
        {
            dont_read = 1;
            if (single_string || double_string)
                THROW(QUOTES_ERR)
        }

        if (((tmp != '\n') 
            ||((tmp == '\n') && (single_string || double_string)))
           &&(!dont_read))
        {
            str[pos++] = tmp;
        }
        
        do_nothing = 0;
        if (tmp == '\\')
            do_nothing = 1;
    }

    str[pos] = '\0';
    str = (char*)realloc(str,sizeof(char)*(pos+1));
    if (str == NULL)
        THROW(MEMORY_ERR)
    
    return str;
}

/*
 * пользовательские данные
 */

struct profile
{
    char* name;
    char* directory;
    Dict* dictionary;
}user;

void collect_data()
{
    FILE *fp = fopen("shprofile","r");
    if(fp == NULL)
    {
        fp = fopen("shprofile","w");
        if (fp == NULL)
            THROW(FILE_ERR)
        printf("Enter your name > ");
        user.name = (char*)malloc(sizeof(char)*256);
        fgets(user.name,255,stdin);
        if (user.name[strlen(user.name)-1] == '\n')
            user.name[strlen(user.name)-1] = '\0';
        fputs(user.name,fp);
    }else
    {
        user.name = read_long_line(fp);
    }
    user.dictionary = dict_init();
    dict_append(user.dictionary,"$USER",user.name);
    fclose(fp);
}

void free_data()
{
    dict_clear(&user.dictionary);
    free(user.name);
}

/*
 * подстановка переменных
 */

void strcut(char* str,int pos,int len)
{
    int i;
    for(i=pos;str[i+len];i++)
        str[i] = str[i+len];
    str[i] = '\0';
}

void insert_vars(char **str,Dict *d)
{
    int pos;
    int i;
    int j;
    char val = 0;
    char *value;
    char *key;
    char *tmp;
    char single_string = 0;
    char double_string = 0;
    char do_nothing = 0;

    if(*str==NULL)
        return;
    i = 0;
    while(1)
    {
        if (( (*str)[i]=='$')&&(!single_string))
        {
            pos = i;
            val = 1;
        }

        if (((*str)[i] == '\'')&&(!double_string)&&(!do_nothing))
            single_string = !single_string;
        
        if (((*str)[i] == '\"')&&(!single_string)&&(!do_nothing))
            double_string = !double_string;

        if((((((*str)[i] == '\'') || ((*str)[i] == '\"'))&&(!do_nothing)) 
                    || ((*str)[i] == ' ')
                    ||( (*str)[i] == '\0')) 
                &&val 
                &&(!single_string))
        {
            key = (char*)malloc(sizeof(char)*(i-pos+1));
            for(j=pos;j<i;j++)
                key[j-pos] = (*str)[j];
            key[j-pos] = '\0';
            
            value = dict_get(d,key);
            *str = (char*)realloc(*str,sizeof(char)*(strlen(*str) + strlen(value) - strlen(key)+1));
            strcut(*str,pos+1,i-pos-1);
            
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

         
        if (((((*str)[i]=='\'')&&(single_string || !double_string))
            ||(((*str)[i]=='\"')&&(double_string || !single_string)))
            &&(!do_nothing))
        {
            strcut(*str,i,1);
            i--;
        }

        if ((i>=0)&&((*str)[i] == '\\')&&(!do_nothing))
        {
            strcut(*str,i,1);
            do_nothing = 1;
        }
        else
            do_nothing = 0;
        i++;
    }
}

/*
 * получение парметров для запуска программы argc и argv 
 */

void split(char* str, int *argc,char ***argv,Dict *d)
{
    int i;
    char single_string = 0,double_string = 0;
    int size = 0,max_size = 1;
    char do_nothing = 0;
    char dont_read = 0;
    for(i=strlen(str)-1;(i>=0)&&(str[i]==' ') ;i--)
        ;
    str[i+1] = '\0';

    if (str[0] == '\0')
    {    
        *argc = -1;
        return;
    }

    *argc = 1;
    *argv = (char**)malloc(sizeof(char*));
    (*argv)[0] = (char*)malloc(sizeof(char)*(max_size+1));
    
    for(i=0;str[i];i++)
    {
        if( (str[i] == '\'')&&(!double_string)&&(!do_nothing) )
        {
            single_string = !single_string;
        }
        
        if( (str[i]=='\"')&&(!single_string)&&(!do_nothing) )
        {
            double_string = !double_string;
        }

        if( (str[i] == ' ')&&(!(single_string || double_string))&&(!do_nothing) )
        {
            if (size!=0)
            {
                (*argv)[(*argc)-1][size] = '\0';
                (*argv)[(*argc)-1] = (char*)realloc((*argv)[(*argc)-1],sizeof(char)*(size+1));
                insert_vars(&(*argv)[(*argc)-1],d);

                size = 0;
                max_size = 1;
                *argc = (*argc) + 1;
                *argv = (char**)realloc(*argv,sizeof(char*)*(*argc));
                (*argv)[(*argc)-1] = (char*)malloc(sizeof(char)*(max_size+1));
            }
            continue;
        }
       
        if ((str[i]!='\\') || ((str[i]=='\\')&&(do_nothing)))
            (*argv)[(*argc)-1][size++] = str[i];
       
        if ((str[i] == '\\')&&(!do_nothing))
        {
            do_nothing = 1;
        }else{
            do_nothing = 0;
        } 
       
        if (size==max_size)
        {
            max_size<<=1;
            (*argv)[(*argc)-1] = (char*)realloc( (*argv)[(*argc)-1],sizeof(char)*(max_size+1));
        }        
    }
    if (size!=0)
    {
        (*argv)[(*argc)-1][size] = '\0';
        insert_vars(&(*argv)[(*argc)-1],d);
    }
}

/*
 * все функции работают сдесь 
 */

int main()
{
    int i;
    int argc = -1;
    char **argv = NULL;
    TRY
    {
        
        collect_data();
        char *str = NULL;
        while(1)
        {
            printf("%s$ ",user.name);
            str = read_long_line(stdin);
            
            split(str,&argc,&argv,user.dictionary);
            for(i=0;i<argc;i++)
                printf("%i %s\n",i,argv[i]);
                                   
            for(i=0;i<argc;i++)
                free(argv[i]);
            if(argc>=0)
                free(argv);
            
            free(str);
            
            if (feof(stdin))
                break;   
        }
    }
    CATCH(MEMORY_ERR)
    {
        perror(MEMORY_ERR_TEXT);
    }
    CATCH(FILE_ERR)
    {
        perror(FILE_ERR_TEXT);
    }
    CATCH(QUOTES_ERR)
    {
        perror(QUOTES_ERR_TEXT);
    }
    ENDTRY
    free_data();
    return 0;
}
