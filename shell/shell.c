#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>

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

#define EXIT    2
#define ERROR   1
#define SUCCESS 0

#define APPEND  1
#define REWRITE 2


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
 * структура программы и работа со структурой
 */

struct program
{
    char *name;
    strarr *args;
    char *input_file,*output_file;
    int output_type;
};

typedef struct program Program;

Program * program_init()
{
    Program *prog = (Program*)malloc(sizeof(Program));
    prog->name = NULL;
    prog->args = NULL;
    prog->input_file = NULL;
    prog->output_file = NULL;
    return prog;
}

void program_clean(Program *prog)
{
    
    strarr_clear(prog->args);
    if (prog->input_file!=NULL)
        free(prog->input_file);
    if (prog->output_file!=NULL)
        free(prog->output_file);
    free(prog);
}

void program_print(Program *prog)
{
    int i;

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
}

struct job
{
    Program **program;
    int number_of_programs;
};

typedef struct job Job;

Job *job_init()
{
    Job *j = (Job*)malloc(sizeof(Job));
    j->program = NULL;
    j->number_of_programs = 0;
    return j;
}

void job_clean(Job *j)
{
    int i;
    
    for (i = 0;i < j->number_of_programs; i++)
        program_clean(j->program[i]);
    free(j);
}

void job_push(Job *j,Program *prog)
{
    j->number_of_programs += 1;
    j->program = (Program**)realloc(j->program,sizeof(Program*)*(j->number_of_programs));
    j->program[j->number_of_programs-1] = prog;
    return;
}

void job_print(Job *j)
{
    int i;
    for (i = 0;i<j->number_of_programs;i++)
        program_print(j->program[i]);
}

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

/*
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
 * разбиение аргументов на подпрограммы
 * работа с конвеером
 */

void execute(Program *prog,int fd0,int fd1)
{
    if (fork() == 0)
    {
        if (prog->input_file != NULL)
        {
            close(fd0);
            fd0 = open(prog->input_file,O_RDONLY);
            if (fd0<0)
                return;
        }

        if (prog->output_file != NULL)
        {
            close(fd1);
            if (prog->output_type == REWRITE)
                fd1 = open(prog->output_file, O_CREAT|O_WRONLY|O_TRUNC);
            else
                fd1 = open(prog->output_file, O_CREAT|O_WRONLY|O_APPEND);
            
            if (fd1<0)
            {
                printf("No such file\n");
                return;
            }
        }
       
        dup2(fd0,0);
        dup2(fd1,1);
        
        if (fd0 != 0)
            close(fd0);
        if (fd1 != 1)
            close(fd1);

        strarr_push(prog->args,NULL);
        execvp(prog->args->argv[0],prog->args->argv);
        printf("No such command\n");
        exit(1);
    
    }else
    {
        if (fd0 != 0)
            close(fd0);
        if (fd1 != 1)
            close(fd1);
        wait(NULL);
    }
}


void run_conveyor(strarr* args)
{
    Job *j = job_init();
    Program *prog = program_init();
    
    int start = 0,
        end = 0,
        conveyor = 0,
        end_of_args = 0,
        last_arg = 0,
        i = 0;

    int **pipes;
    
    if (prog == NULL)
        THROW(MEMORY_ERR)
    
    if (args == NULL)
        THROW(MEMORY_ERR)

    while(end < args->argc)
    {  
        if (end == args->argc - 1)
            last_arg = 1;
        
        if (!strcmp(args->argv[end],"|"))
            conveyor = 1;
        
        if (!strcmp(args->argv[end],">>"))
        {
            if (last_arg)
            {
                job_clean(j);
                program_clean(prog);
                return;
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
                job_clean(j);
                program_clean(prog);
                return;
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
                job_clean(j);
                program_clean(prog);
                return;
            }
            
            free(strarr_pop(args,end));
            prog->input_file = strarr_pop(args,end); 
            
            if (end == args->argc)
                end_of_args = 1;
            else
                continue;
        }

        if(conveyor||last_arg||end_of_args)
        {
            if ((last_arg)&&(!conveyor))
                end++;

            if (((start == end)||(last_arg))&&(conveyor))
            {
                job_clean(j);
                program_clean(prog);
                return;
            }

            prog->args = strarr_slice(args,start,end);
            job_push(j,prog);
            prog = program_init();
            
            conveyor = 0;
            start = end+1;
        }

        end++;
    }
    program_clean(prog); 
    
    if (j->number_of_programs > 0)
    {
        pipes = (int**)malloc(sizeof(int*)*(j->number_of_programs));
        pipes[0] = (int*)malloc(sizeof(int)*2);
        pipe(pipes[0]);
        pipes[0][0] = 0;

        for (i = 0;i < j->number_of_programs-1;i++)
        {
            pipes[i+1] = (int*)malloc(sizeof(int)*2);
            pipe(pipes[i+1]);
            execute(j->program[i],pipes[i][0],pipes[i+1][1]);
        }

        execute(j->program[i],pipes[i][0],1);
        close(pipes[i][1]);
    }

    job_clean(j);
    return;
}

void run(strarr *args)
{
    int start = 0,
        end = 0;

    while (end < args->argc)
    {
        if (!strcmp(args->argv[end],";"))
        {
            run_conveyor(strarr_slice(args,start,end));
            end++;
            start = end;
        }
        end++;
    }
    if (start!=end)
        run_conveyor(strarr_slice(args,start,end));
}

/*
 * все функции работают здесь 
 */

int main()
{
    strarr *args;
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
            
            run(args);

            strarr_clear(args);
            free(str);
            
            if (feof(stdin))
            {
                printf("Bye Bye!\n");
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
