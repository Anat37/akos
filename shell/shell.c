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
#define MEMORY_ERR_TEXT "Some error with memory"
#define FILE_ERR_TEXT "Some error with file access"

/*
 * считывает команду произвольного размера
 */ 

char* read_long_line(FILE* infp)
{
    size_t max_size = 1,pos = 0;
    char *str = (char*)malloc(sizeof(char)*max_size);
    char tmp;
    char single_string = 0;
    char double_string = 0;

    if (infp == NULL)
        THROW(FILE_ERR)

    if (str == NULL)
        THROW(MEMORY_ERR)
    
    if (feof(infp))
        return NULL;

    while(fread(&tmp,sizeof(char),1,infp))
    {
        if(pos==max_size)
        {
            max_size<<=1;
            str = (char*)realloc(str,sizeof(char)*max_size);
            if (str == NULL)
                THROW(MEMORY_ERR)
        }
        
        if ( (tmp=='\'') && (!double_string) )
            single_string = (single_string)?0:1;

        if ( (tmp=='\"') && (!single_string) )
            double_string = (double_string)?0:1;

        if ( (tmp=='\n') && (!(single_string || double_string) ))
            break;

        if ((tmp =='\n')&&(single_string || double_string))
            printf(" > ");

        str[pos] = tmp;
        pos++;
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
        user.name = read_long_line(stdin);
        fputs(user.name,fp);
    }else
    {
        user.name = read_long_line(fp);
    }
    fclose(fp);
}



/*
 * подстановка переменных
 */

void insert_vars(char** str)
{
    printf("Trying to insert variables\n");
}

/*
 * получение парметров для запуска программы argc и argv 
 */

void split(char* str, int *argc,char ***argv)
{
    char single_string = 0,double_string = 0;
    int size = 0,max_size = 1;
    int i;
    
    for(i=strlen(str)-1;(str[i]==' ')&&(i>=0) ;i--)
        ;
    str[i+1] = '\0';

    if (str[0] == '\0')
    {    
        *argc = -1;
        return;
    }
     
    *argc = 1;
    *argv = (char**)malloc(sizeof(char*));
    *argv[0] = (char*)malloc(sizeof(char)*(max_size+1));
    
    for(i=0;str[i];i++)
    {
        if( (str[i] == '\'')&&(!double_string) )
        {
            single_string = !single_string;
        }
        
        if( (str[i]=='\"')&&(!single_string) )
        {
            if (double_string)
                insert_vars(&(*argv)[(*argc)-1]);
            double_string = !double_string;
        }

        if( (str[i] == ' ')&&(!(single_string || double_string)) )
        {
            if (size!=0)
            {
                (*argv)[(*argc)-1][size] = '\0';
                (*argv)[(*argc)-1] = (char*)realloc((*argv)[(*argc)-1],sizeof(char)*(size+1));
                
                size = 0;
                max_size = 1;
                *argc = (*argc) + 1;
                *argv = (char**)realloc(*argv,sizeof(char*)*(*argc));
                (*argv)[(*argc)-1] = (char*)malloc(sizeof(char)*(max_size+1));
            }
            continue;
        }
        
        (*argv)[(*argc)-1][size] = str[i];
        size++;
        if (size==max_size)
        {
            max_size<<=1;
            (*argv)[(*argc)-1] = (char*)realloc( (*argv)[(*argc)-1],sizeof(char)*(max_size+1));
        }        
    }
    
    if (size!=0)
        (*argv)[(*argc)-1][size] = '\0';
}

/*
 *
 */

int main()
{
    int i;
    int argc;
    char **argv;
    TRY
    {
        collect_data();
        char *str = NULL;
        do
        {
            printf("%s$ ",user.name);
            str = read_long_line(stdin);
            
            split(str,&argc,&argv);
            printf("splited data:\n");
            for(i=0;i<argc;i++)
                printf("%i %s\n",i,argv[i]);

        }while( (!feof(stdin)) && (strcmp(str,"exit")) );
    }
    CATCH(MEMORY_ERR)
    {
        perror(MEMORY_ERR_TEXT);
    }
    CATCH(FILE_ERR)
    {
        perror(FILE_ERR_TEXT);
    }
    ENDTRY

    return 0;
}
