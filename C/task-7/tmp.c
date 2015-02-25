#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void execute(int n,int fd0,int fd1)
{
    if (fork() == 0)
    {
        dup2(fd0,0);
        dup2(fd1,1);
        
        if (fd0!=0)
            close(fd0);
        if (fd1!=1)
            close(fd1);

        if (n == 0)
        {
            execlp("ls","ls",NULL);
            exit(1);
        }
        
        if (n == 1)
        {
            execlp("wc","wc",NULL);
            exit(1);
        }
    }else{
        if (fd0!=0)
            close(fd0);
        if (fd1!=1)
            close(fd1);
    }
}


int main()
{
    int fd[2],fd1[2];
    pipe(fd);

    execute(0,0,fd[1]);
    execute(1,fd[0],1);
    return 0;
}
