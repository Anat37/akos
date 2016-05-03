#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    int fd[2],fd1[2];
    int pid,pid1;
    
    pipe(fd);
    pid = fork();
    
    if (pid == 0)
    {
        dup2(fd[1],1);
        close(fd[0]);
        close(fd[1]);
        execlp("ls","ls",NULL);
        return -1;
    }
    
    dup2(fd[0],0);
    close(fd[0]);
    close(fd[1]);
    
    pipe(fd1);
    pid1 = fork();
    if (pid1 == 0)
    {
        dup2(fd1[1],1);
        close(fd1[0]);
        close(fd1[1]);

        execlp("wc","wc",NULL);
        return -1;
    }
    
    dup2(fd1[0],0);
    close(fd1[0]);
    close(fd1[1]);

    execlp("wc","wc",NULL);
    return 0;
}
