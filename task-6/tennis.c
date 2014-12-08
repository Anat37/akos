#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

pid_t grandpa,pa,child;
pid_t next;
int pd[2];


struct sigaction action;

#define strsize 256
#define lessthan 11

void reverse(char *str)
{
    int i,
        j;
    char c;

    for(i=0,j=strlen(str)-1; i<j ; i++,j--)
    {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

void itoa(int n, char *str)
{
    int i=0;
    if (n == 0)
    {
        str[0] = '0';
        i = 1;
    }

    while((n) && (i < strsize))
    {
        str[i] = '0' + n%10;
        n /= 10;
        ++i;
    }
    str[i] = '\0';
    reverse(str);
}


void get_msg(int s)
{
    sleep(1);
    int msg;
    char buf[strsize];
    char num[strsize];

    read(pd[0],&msg,sizeof(int));
    
    itoa(getpid(),buf);
    strcat(buf, " have got ");
    itoa(msg,num);
    strcat(buf,num);
    strcat(buf,"\n");
    write(1,buf,strlen(buf));

    msg++;
    write(pd[1],&msg,sizeof(int));
    
    if (msg!=lessthan)
        kill(next,SIGUSR1);
    else
    {
        kill(grandpa,SIGINT);
        kill(pa,SIGINT);
        kill(child,SIGINT);
    }
}

void shut_down(int c)
{
    char buf[strsize];
    itoa(getpid(),buf);
    strcat(buf," is done\n");
    write(1,buf,strlen(buf));
    exit(0);
}

int main()
{
    int msg;
    pipe(pd);

    action.sa_handler = shut_down;
    sigaction(SIGINT,&action,NULL);  
    action.sa_handler = get_msg;
    sigaction(SIGUSR1,&action,NULL);

    grandpa = getpid();
    next = grandpa;
    pa = fork();
    
    if ( pa == 0 )
    {
        child = fork();
        if (child)
            next = child;
    }else
    {
        next = pa;
        msg = 0;
        write(pd[1],&msg,sizeof(int));
        sleep(1);
        kill(next,SIGUSR1);
    }
    while(1)
        pause();
    
    return 0;
}
