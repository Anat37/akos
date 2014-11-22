#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pid;
    char *argv[] = {"ls","-l",(char*)0};
    if ((pid = fork())==0)
    {
        execv("/bin/ls",argv);
        printf("ERROR!");
    }
    return 0;
}
