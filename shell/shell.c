#ifndef UNISTD
    #include <unistd.h>
    #define UNISTD
#endif

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
#include "strarr.c"
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

#define EXIT  2
#define ERROR 1
#define SUCCESS 0

/*
 * считывает команду произвольного размера
 */ 

char* read_long_line(FILE* infp)
{
    int max_size = 1,
        pos = 0;
    char *str = (char*)malloc(sizeof(char)*max_size);
    char tmp,
         single_string = 0,
         double_string = 0,
         do_nothing = 0,
         dont_read = 0;
    
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
        if (pos == max_size)
        {
            max_size<<=1;
            str = (char*)realloc(str,sizeof(char)*max_size);
            if (str == NULL)
                THROW(MEMORY_ERR)
        }

        if ((tmp == '\'')
                &&(!double_string)
                &&(!do_nothing))
        {
            single_string = (single_string)?0:1;
        }
        
        if ((tmp == '\"')
                &&(!single_string) 
                &&(!do_nothing))
        {
            double_string = (double_string)?0:1;
        }

        if ((tmp == '\n') 
                &&(!(single_string || double_string))
                &&(!do_nothing))
        {
            break;
        }

        if ((tmp =='\n')
                &&(single_string || double_string || do_nothing))
        {
            printf(" > ");
        }
        
        if ((tmp == '#')
                &&(!do_nothing))
        {
            dont_read = 1;
            if (single_string || double_string)
                THROW(QUOTES_ERR)
        }

        if (((tmp != '\n') 
                ||((tmp == '\n')
                    &&(single_string || double_string)))
            &&(!dont_read))
        {
            if ((tmp == 'n')&&(do_nothing))
                str[pos] = '\n';
            else
                str[pos] = tmp;
            pos++;
        }
        
        do_nothing = 0;
        if (tmp == '\\')
            do_nothing = 1;
    }

    str[pos] = '\0';
    str = (char*)realloc(str,sizeof(char)*(pos+1));
    if (str == NULL)
    {
        THROW(MEMORY_ERR)
    }
    return str;
}

/*
 * пользовательские данные
 */

struct profile
{
    char* name;
    Dict* dictionary;
};

typedef struct profile Profile;

Profile* collect_data()
{
    Profile* user = (Profile*)malloc(sizeof(Profile));
    FILE *fp = fopen("shprofile","r");
    if (fp == NULL)
    {
        fp = fopen("shprofile","w");
        if (fp == NULL)
            THROW(FILE_ERR)
        printf("Enter your name > ");
        user->name = (char*)malloc(sizeof(char)*256);
        if (user->name == NULL)
            THROW(MEMORY_ERR)
        fgets(user->name,255,stdin);
        if (user->name[strlen(user->name)-1] == '\n')
            user->name[strlen(user->name)-1] = '\0';
        fputs(user->name,fp);
    }else
        user->name = read_long_line(fp);
    user->dictionary = dict_init();
    dict_append(user->dictionary,"$USER",user->name);
    fclose(fp);
    return user;
}

void free_data(Profile *user)
{
    dict_clear(&user->dictionary);
    free(user->name);
    free(user);
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
    int pos,
        i,
        j;
    char *value,
         *key,
         *tmp,
         single_string = 0,
         double_string = 0,
         do_nothing = 0,
         val = 0;

    if(*str == NULL)
        return;
    i = 0;
    while(1)
    {
        if (((*str)[i]=='$')
                &&(!single_string))
        {
            pos = i;
            val = 1;
        }

        if (((*str)[i] == '\'')
                &&(!double_string)
                &&(!do_nothing))
        {
            single_string = !single_string;
        }
        
        if (((*str)[i] == '\"')
                &&(!single_string)
                &&(!do_nothing))
        {
            double_string = !double_string;
        }

        if ((((((*str)[i] == '\'')||((*str)[i] == '\"'))
                        &&(!do_nothing)) 
                    ||((*str)[i] == ' ')
                    ||((*str)[i] == '\0')) 
                &&(val) 
                &&(!single_string))
        {
            key = (char*)malloc(sizeof(char)*(i-pos+1));
            if (key == NULL)
                THROW(MEMORY_ERR)
            for(j = pos;j<i;j++)
                key[j-pos] = (*str)[j];
            key[j-pos] = '\0';
            
            value = dict_get(d,key);
            
            *str = (char*)realloc(*str,sizeof(char)*(strlen(*str) + strlen(value) - strlen(key)+1));
            if (*str == NULL)
                THROW(MEMORY_ERR)
            strcut(*str,pos+1,i-pos-1);
            
            tmp = (char*)malloc(sizeof(char)*(strlen(*str)-pos));
            if (tmp == NULL)
                THROW(MEMORY_ERR)
            strcpy(tmp,&(*str)[pos+1]);
            
            (*str)[pos] = '\0';
            strcat(*str,value);
            strcat(*str,tmp);
            
            free(tmp);
            free(key);
            i = pos;
            val = 0;
            continue;
        }

        if (((((*str)[i]=='\'')
                &&(single_string || !double_string))
            ||(((*str)[i]=='\"')
                &&(double_string || !single_string)))
            &&(!do_nothing))
        {
            strcut(*str,i,1);
            continue;
        }

        if (((*str)[i] == '\\')
                &&(!do_nothing))
        {
            strcut(*str,i,1);
            do_nothing = 1;
            continue;
        }
        else
            do_nothing = 0;
        
        if ((*str)[i] == '\0')
            break;
        i++;
    }
}

/*
 * получение парметров для запуска программы argc и argv 
 */

strarr* split(char* str, Dict *d)
{
    strarr *res = strarr_init();
    int i,
        size = 0,
        max_size = 1;
    char *tmp = (char*)malloc(sizeof(char)*max_size),
         single_string = 0,
         double_string = 0,
         do_nothing = 0,
         dont_read = 0;
    
    for(i = strlen(str)-1;(i >= 0)&&(str[i] == ' ');i--)
        ;
    str[i+1] = '\0';
    
    for(i = 0;str[i] == ' ';)
        strcut(str,i,1);

    if (str[0] == '\0')
    {    
        return res;
    }

    for(i=0;str[i];i++)
    {
        if ((str[i] == '\'')
                &&(!double_string)
                &&(!do_nothing) )
        {
            single_string = !single_string;
        }
        
        if ((str[i]=='\"')
                &&(!single_string)
                &&(!do_nothing) )
        {
            double_string = !double_string;
        }

        if (((str[i] == ' ')||(str[i] == ';')||(str[i] == '<')||(str[i] == '>'))
                &&(!(single_string || double_string))
                &&(!do_nothing))
        {
            if (size != 0)
            {
                tmp[size] = '\0';
                tmp = (char*)realloc(tmp,sizeof(char)*(size+1));
                if (tmp == NULL)
                    THROW(MEMORY_ERR)
                insert_vars(&tmp,d);
                
                strarr_push(res,tmp);
                size = 0;
                max_size = 1;
                free(tmp);
                tmp = (char*)malloc(sizeof(char)*max_size);
                if (tmp == NULL)
                    THROW(MEMORY_ERR)
            }

            if (str[i] == ';')
                strarr_push(res,";");
            if (str[i] == '<')
                strarr_push(res,"<");
            if (str[i] == '>')
            {
                if (str[i+1] == '>')
                {
                    i++;
                    strarr_push(res,">>");
                }else
                {
                     strarr_push(res,">");
                }
            }
            
            continue;
        }
       
        tmp[size++] = str[i];
       
        if ((str[i] == '\\')
                &&(!do_nothing))
            do_nothing = 1;
        else
            do_nothing = 0;
       
        if (size == max_size)
        {
            max_size<<=1;
            tmp = (char*)realloc(tmp ,sizeof(char)*(max_size+1));
            if (tmp == NULL)
                THROW(MEMORY_ERR)
        }
    }
    if (size!=0)
    {
        tmp[size]  = '\0';
        insert_vars(&tmp,d);
        strarr_push(res,tmp);
    }
    
    return res;
}

/*
 * выполнение команды
 */


int execute(strarr *args,int start,int end)
{
    strarr* tmp;
    
    int pid = 0,
        status = 0;

    tmp = strarr_slice(args,start,end);
    strarr_push(tmp,NULL);
    
    if((pid = fork()) == 0)
    {
        execvp(tmp->argv[0],tmp->argv);
        strarr_clear(tmp);
        return ERROR;
    }else{
        wait(&status);
    }
    
    strarr_clear(tmp);
    return SUCCESS;
}

int analyze(strarr* args)
{
    int start = 0,
        end = 0,
        status = 0;
    
    if ((args->argc > 0)&&(!strcmp(args->argv[0],"exit")))
        return EXIT;

    while(end < args->argc)
    {
        if((!strcmp(args->argv[end],";"))||(end == args->argc - 1))
            {
               if ((end == args->argc-1)&&(strcmp(args->argv[end],";")))
                    end++;
               
               if (execute(args,start,end) != SUCCESS)
                   return ERROR;
               start = end+1;
            }
            end++;
     }
    return SUCCESS;
}

/*
 * все функции работают сдесь 
 */

int main()
{
    strarr *args;
    int i,
        status;
    Profile* user;
    char *str = NULL;
    
    TRY
    {
        user = collect_data(); 
        str = NULL;
        while(1)
        {
            printf("%s$ ",user->name);
            str = read_long_line(stdin);
            args = split(str,user->dictionary);

            //for(i =0;i< args->argc;i++)
              //  printf("%s\n",args->argv[i]);

            status = analyze(args);   
            
            strarr_clear(args);
            free(str);
            
            if (feof(stdin))
                break;  
            
            if (status == EXIT)
            {
                printf("Bye Bye!\n");
                break;
            }

            if (status == ERROR)
            {
                printf("Shit happends\n\tForest Ghump\n");
                break;
            }
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
    free_data(user);
    return 0;
}
