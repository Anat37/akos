#ifndef UNISTD
    #include <unistd.h>
    #include <sys/wait.h>
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
#include "strarr.h"

/* 
 * описание конструкции try catch
 */

static jmp_buf err;

#define TRY                 \
    {                       \
        switch(setjmp(err)) \
        {                   \
            case 0:

#define CATCH(x)       \
                break; \
            case x:

#define ENDTRY \
        }      \
    }

#define THROW(x) longjmp(err,x);

/*
 * описание исключений
 */

#define MEMORY_ERR 1
#define FILE_ERR   2
#define QUOTES_ERR 3

#define MEMORY_ERR_TEXT  "Some error with memory"
#define FILE_ERR_TEXT    "Some error with file access"
#define QUOTES_ERR_TEXT  "Some error with quotes"

#define EXIT  2
#define ERROR 1
#define SUCCESS 0

#define APPEND 1
#define REWRITE 2

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
         comment = 0;
    
    if (str == NULL)
        THROW(MEMORY_ERR)
    
    if (infp == NULL)
    {
        free(str);
        THROW(FILE_ERR)
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
                &&(!do_nothing)
                &&(!(single_string||double_string)))
        {
            comment = 1;
        }

        if (((tmp != '\n') 
                ||((tmp == '\n')
                    &&(single_string || double_string)))
            &&(!comment))
        {
            if ((tmp == 'n')&&(do_nothing))
                str[pos] = '\n';
            else
                str[pos] = tmp;
            pos++;
        }
        
        if ((tmp == '\\')&&(!do_nothing))
            do_nothing = 1;
        else
            do_nothing = 0;
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
    user->name = (char*)malloc(sizeof(char)*256);
    strcpy(user->name,getenv("USER"));
    user->dictionary = dict_init();
    dict_append(user->dictionary,"$USER",user->name);
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
            
            value = (char*)malloc(sizeof(char)*(strlen(key)+1));
            strcpy(value,key);
            /*value = dict_get(d,key);*/
            /*printf("%s %s\n",key,getenv(key));*/
            
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
            free(value);
            i = pos+1;
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

/**
 * получение парметров для запуска программы argc и argv
 * 
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
         do_nothing = 0;
    
    for(i = strlen(str)-1;(i >= 0)&&(str[i] == ' ');i--)
        ;
    str[i+1] = '\0';
    
    for(i = 0;str[i] == ' ';i++)
        ;
    strcut(str,0,i);

    if (str[0] == '\0')
    {  
        free(tmp);
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
       
        if (((str[i] == ' ')||(str[i] == ';')||(str[i] == '<')||(str[i] == '>')||(str[i] == '|')||(str[i] == '&'))
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
            
            if (str[i] == '&')
                strarr_push(res,"&");
            
            if (str[i] == '|')
                strarr_push(res,"|");

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
    free(tmp);
    return res;
}

/*
 * выполнение команды
 */

struct program
{
    char *name;
    strarr *args;
    char *input_file,*output_file;
    int output_type;
};

typedef struct program Program;

void program_clean(Program *prog)
{
    
    strarr_clear(prog->args);
    if (prog->input_file!=NULL)
        free(prog->input_file);
    if (prog->output_file!=NULL)
        free(prog->output_file);
    free(prog);
}

int analyze(strarr* args)
{
    Program *prog = (Program*)malloc(sizeof(Program));
    int start = 0,
        end = 0,
        i = 0,
        delimeter = 0,
        conveyor = 0,
        met_conveyor = 0,
        end_of_args = 0,
        last_arg = 0,
        ampersand = 0;
    
    if (prog == NULL)
        THROW(MEMORY_ERR)

    prog->args = NULL;
    prog->input_file = NULL;
    prog->output_file = NULL;

    if ((args->argc > 0)&&(!strcmp(args->argv[0],"exit")))
    {
        program_clean(prog);
        return EXIT; 
    }
    
    while(end < args->argc)
    {  
        if (end == args->argc - 1)
            last_arg = 1;
        
        if (!strcmp(args->argv[end],";"))
            delimeter = 1;

        if (!strcmp(args->argv[end],"|"))
            conveyor = 1;
        
        if (!strcmp(args->argv[end],"&"))
            ampersand = 1;

        if (!strcmp(args->argv[end],">>"))
        {
            if (last_arg)
            {
                program_clean(prog);
                return ERROR;
            }

            free(strarr_pop(args,end));
            prog->output_file = strarr_pop(args,end); 
            prog->output_type = APPEND;
            
            if (end == args->argc)
                end_of_args = 1;
            else
                continue;
        }
        
        if ((!end_of_args)&&(!strcmp(args->argv[end],">")))
        {
            if (last_arg)
            {
                program_clean(prog);
                return ERROR;
            }
            free(strarr_pop(args,end));
            prog->output_file = strarr_pop(args,end); 
            prog->output_type = REWRITE;
            
            if (end == args->argc)
                end_of_args = 1;
            else
                continue;
        }
        
        if ((!end_of_args)&&(!strcmp(args->argv[end],"<")))
        {
            if (last_arg)
            {
                program_clean(prog);
                return ERROR;
            }
            
            free(strarr_pop(args,end));
            prog->input_file = strarr_pop(args,end); 
            
            if (end == args->argc)
                end_of_args = 1;
            else
                continue;
        }

        if(delimeter||conveyor||ampersand||last_arg||end_of_args)
        {
            if ((last_arg)&&(!delimeter)&&(!conveyor)&&(!ampersand))
                end++;

            if ((((start == end)||(last_arg))&&(conveyor))||((ampersand)&&(!met_conveyor)))
            {
                program_clean(prog);
                return ERROR;
            }

            prog->args = strarr_slice(args,start,end);
            
            if (prog->args->argc>0)
            {
                printf("name = %s\n",prog->args->argv[0]);
                if (prog->output_file != NULL)
                    printf("output_file = %s with type = %i\n",prog->output_file,prog->output_type);
                if (prog->input_file != NULL)
                    printf("input_file = %s\n",prog->input_file);
            }

            for(i=0;i < prog->args->argc; i++)
                printf("arg[%i] = %s\n",i,prog->args->argv[i]);
            
            program_clean(prog);
            prog = (Program*)malloc(sizeof(Program));
            prog->args = NULL;
            prog->input_file = NULL;
            prog->output_file = NULL;
            
            if (conveyor)
                met_conveyor = 1;
            else
                met_conveyor = 0;

            delimeter = 0;
            conveyor = 0;
            
            start = end+1;
        }

        end++;
    }
    
        
    program_clean(prog);
    return SUCCESS;
}

/*
 * все функции работают сдесь 
 */

int main()
{
    strarr *args;
    int status;
    Profile* user;
    char *str = NULL;
    
    TRY
    {
        user = collect_data();
        
        str = NULL;
        while(1)
        {
            /*printf("%s$ ",user->name);*/
            str = read_long_line(stdin);
            
            args = split(str,user->dictionary);
           
            
            status = analyze(args);
             
            strarr_clear(args);
            free(str);
            
            if ((status == EXIT)||(feof(stdin)))
            {
                printf("Bye Bye!\n");
                break;
            }

            if (status == ERROR)
            {
                printf("Error\n");
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
