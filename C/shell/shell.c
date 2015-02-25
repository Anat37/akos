#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include "struct/strarr.h"
#include "struct/program.h"
#include "struct/job.h"
#include "struct/profile.h"

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

Profile *user;
Job *j;

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

void insert_vars(char **str)
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
            key = (char*)malloc(sizeof(char)*(i-pos));
            if (key == NULL)
                THROW(MEMORY_ERR)
            for(j = pos+1;j<i;j++)
                key[j-pos-1] = (*str)[j];
            key[j-pos-1] = '\0';
            
            value = getenv(key);
            if (value == NULL)
                value = "";
            
            *str = (char*)realloc(*str,sizeof(char)*(strlen(*str) + abs(strlen(value) - strlen(key))+1));
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
            
            *str = (char*)realloc(*str,sizeof(char)*(strlen(*str)+1));
            if (*str == NULL)
                THROW(MEMORY_ERR)
            
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
 * 
 */

Strarr* split(char* str)
{
    Strarr *res = strarr_init();
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
                insert_vars(&tmp);
                
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
        insert_vars(&tmp);
        strarr_push(res,tmp);
    }
    free(tmp);
    return res;
}

/*
 * разбиение аргументов на подпрограммы
 * работа с конвеером
 */

int get_var(char* str)
{
    int i,
        j,
        status = 0;
    char** ans;

    for (i=0;i<strlen(str);i++)
        if (str[i] == '=')
        {
            status = 1;
            break;
        }

    if (status == 1)
    {
        ans = (char**)malloc(2*sizeof(char*));
        
        if (ans == NULL)
            THROW(MEMORY_ERR)
        
        ans[0] = (char*)malloc(sizeof(char)*(i+1));
        ans[1] = (char*)malloc(sizeof(char)*(strlen(str)-i));

        i = 0;
        while(str[i] != '=')
        {
            ans[0][i] = str[i];
            i++;
        }
        ans[0][i] = '\0';
        i++;

        j = 0;
        while(str[i])
        {
            ans[1][j] = str[i];
            i++;
            j++;
        }
        ans[1][j] = '\0';
        
        setenv((const char*)ans[0],(const char*)ans[1],(int)1);

        free(ans[0]);
        free(ans[1]);
        free(ans);

        return 1;
    }else
    {
        return 0;
    }
}

void handler()
{
    int i;
    for (i=0;i<j->number_of_programs;i++)
    {
        if ( j->program[i]->status == 1)
        {
            /*printf("still working %i\n",j->program[i]->pid);*/
            kill(j->program[i]->pid,SIGINT);
        }
    }
    signal(SIGINT,handler);
}

int execute(Program *prog,int fd0,int fd1)
{
    pid_t pid,tmp;
    int pipe_background[2],i;
    char c;
    if ((fd1 == 1) && (j->background))
        pipe(pipe_background);

    if ((pid=fork()) == 0)
    {
        if ((fd1 == 1) && (j->background))
        {
            fd1 = pipe_background[1];
        }

        if (prog->input_file != NULL)
        {
            close(fd0);
            fd0 = open(prog->input_file,O_RDONLY);
            if (fd0<0)
            {
                printf("Troubles with files\n");
                return EXIT;
            }
        }

        if (prog->output_file != NULL)
        {
            close(fd1);
            if (prog->output_type == REWRITE)
                fd1 = open(prog->output_file, O_CREAT|O_WRONLY|O_TRUNC,0666);
            else
                fd1 = open(prog->output_file, O_CREAT|O_WRONLY|O_APPEND,0666);
            
            if (fd1<0)
            {
                printf("Troubles with files\n");
                return EXIT;
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
        perror("");
        return EXIT;
    }else
    {
        prog->pid = pid;
        prog->status = ON;
        /*printf("pid = %i has born \n",pid);*/
        /*printf("was background = %i\n",j->background);*/
        
        if (fd0 != 0)
            close(fd0);

        if (fd1 != 1)
            close(fd1);

       if (fd1 == 1)
       {
            if (!(j->background))
            {
                while(!(j->done))
                {
                    tmp = wait(NULL);
                    job_turn_off(j,tmp);
                }
            }

            if (j->background)
            {
                    while(!(j->done))
                    {
                        tmp = wait(NULL);
                        job_turn_off(j,tmp);
                    }

                    dup2(pipe_background[0],0);
                    close(pipe_background[0]);
                    close(pipe_background[1]);
                    
                    printf("\nBackground job\n");
                    for (i=0;i<j->number_of_programs;i++)
                        printf("%i ",j->program[i]->pid);
                    printf("\n");
                    while(read(0,&c,1))
                    {
                        write(1,&c,1);
                    }
                    printf("done\n");
                    printf("%s$ ",user->name);
                    return EXIT;
            }
        }

        return SUCCESS;
    }
}

int run_conveyor(Strarr* args, int background)
{
    Program *prog = program_init();
    
    int start = 0,
        end = 0,
        conveyor = 0,
        end_of_args = 0,
        last_arg = 0,
        i = 0,
        i1 = 0;

    int **pipes;
    
    j = job_init();
    j->background = background;

    if (prog == NULL)
        THROW(MEMORY_ERR)
    
    if (args == NULL)
        THROW(MEMORY_ERR)

    if ((args->argc == 1)&&get_var(args->argv[0]))
    {
        job_clean(j);
        program_clean(prog);
        return SUCCESS;
    }

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
                printf("No file\n");
                job_clean(j);
                program_clean(prog);
                return SUCCESS;
            }

            free(strarr_pop(args,end));
            prog->output_file = strarr_pop(args,end); 
            prog->output_type = APPEND;
            
            if (end == args->argc)
                end_of_args = 1;
            else
                continue;
        }
        
        if ((!end_of_args) && (!strcmp(args->argv[end],">")))
        {
            if (last_arg)
            {
                printf("No file\n");
                job_clean(j);
                program_clean(prog);
                return SUCCESS;
            }
            free(strarr_pop(args,end));
            prog->output_file = strarr_pop(args,end); 
            prog->output_type = REWRITE;
            
            if (end == args->argc)
                end_of_args = 1;
            else
                continue;
        }
        
        if ((!end_of_args) && (!strcmp(args->argv[end],"<")))
        {
            if (last_arg)
            {
                printf("No file\n");
                job_clean(j);
                program_clean(prog);
                return SUCCESS;
            }
            
            free(strarr_pop(args,end));
            prog->input_file = strarr_pop(args,end); 
            
            if (end == args->argc)
                end_of_args = 1;
            else
                continue;
        }

        if(conveyor || last_arg || end_of_args)
        {
            if (last_arg && (!conveyor))
                end++;

            if (((start == end) || last_arg) && conveyor)
            {
                perror("!Error!");
                job_clean(j);
                program_clean(prog);
                return SUCCESS;
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

    if ((!(j->background)) || ((j->background)&&(fork() == 0)))
    {
        pipes = (int**)malloc(sizeof(int*)*(j->number_of_programs));
    
        if (j->number_of_programs > 0)
        {
            pipes[0] = (int*)malloc(sizeof(int)*2);
            pipe(pipes[0]);
            pipes[0][0] = 0;

            for (i = 0;i < j->number_of_programs-1;i++)
            {
                pipes[i+1] = (int*)malloc(sizeof(int)*2);
                pipe(pipes[i+1]);
                if (execute(j->program[i],pipes[i][0],pipes[i+1][1]) != SUCCESS)
                {
                    for (i1=0;i1 <= i+1;i1++)
                        free(pipes[i1]);
                    free(pipes);
                    job_clean(j);
                    return EXIT;
                }
            }

            if (execute(j->program[i],pipes[i][0],1) != SUCCESS)
            {
                for (i1=0;i1 <= i;i1++)
                    free(pipes[i1]);
                free(pipes);
                job_clean(j);
                return EXIT;
            }
            close(pipes[i][1]);
        }
    
        for (i = 0;i < j->number_of_programs;i++)
            free(pipes[i]);
        free(pipes);
        
        if (j->background)
        {
            job_clean(j);
            return EXIT;
        }    
    }
    
    job_clean(j);
    return SUCCESS;
}

int run(Strarr *args)
{
    int start = 0,
        end = 0,
        background = 0;
    
    Strarr *tmp;

    while (end < args->argc)
    {
        if ((!strcmp(args->argv[end],";")) || (!strcmp(args->argv[end],"&")))
        {
            if (start!=end)
            {
                if (!strcmp(args->argv[end],"&"))
                    background = 1;

                tmp = strarr_slice(args,start,end);
                if (run_conveyor(tmp,background) != SUCCESS)
                {
                    strarr_clear(tmp);
                    return EXIT;
                }
                strarr_clear(tmp);
                start = end+1;
                background = 0;
            }else
            {
                start = end+1;
            }
        }
        end++;
    }

    if (start!=end)
    {
        tmp = strarr_slice(args,start,end);
        if (run_conveyor(tmp,background) != SUCCESS)
        {
            strarr_clear(tmp);
            return EXIT;
        }
        strarr_clear(tmp);
    }

    return SUCCESS;
}

/*
 * все функции работают здесь 
 */

int main()
{
    Strarr *args;
    char *str = NULL;

    signal(SIGINT,handler);
    TRY
    {   
        user = profile_init();
        
        str = NULL;
        while(1)
        {
            printf("%s$ ",user->name);
            str = read_long_line(stdin);
            
            args = split(str);
            free(str);
            
            if ( run(args) != SUCCESS )
            {
                strarr_clear(args);
                profile_clean(user);
                return 0;
            }

            strarr_clear(args);
            
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
    profile_clean(user);

    return 0;
}
