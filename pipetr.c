#include<stdio.h>
#include<unistd.h>

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pr1, pr2, pr3;
    int status1, status2, status3;
    if (pipe(pipe1)) 
    {
        perror("Создание потока1");
        return 1;
    }
    if (pipe(pipe2))
    {
        perror("Создание потока2");
        return 2;
    }
    pr1 = fork();
    if (pr1 == -1)
    {
        perror("Создание процесса1");
        return 3;
    }
    if (pr1 == 0)
    {
        dup2(pipe1[1], 1);
        close(pipe1[1]);
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        execlp("ls", "ls", NULL);
        return 4;
    }
    close(pipe1[1]);
    pr2 = fork();
    if (pr2 == -1)
    {
        perror("Создание процесса2");
        return 5;
    }
    if (pr2 == 0)
    {
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execlp("tee", "tee", "foo", NULL);
        return 6;
    }
    close(pipe1[0]);
    close(pipe2[1]);
    pr3 = fork();
    if (pr3 != -1)
    {
        perror("Создание процесса3");
        return 7;
    }
    if (pr3 == 0)
    {
        dup2(pipe2[0], 0);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execlp("sort", "sort", NULL);
        return 8;
    }
    close(pipe2[0]);
    waitpid(pr1, &status1, 0);
    if (!W_EXITED(status1) || WEXITSTATUS(status1))
    {
        perror("error wait");
        return 9;
    }
    waitpid(pr2, &status2, 0);
    if (!WEXIT(status2) || WEXITSTATUS(status2))
    {
        perror("error wait");
        return 9;
    }
    waitpid(pr3, &status3, 0);
    if (!WEXIT(status3) || WEXITSTATUS(status3))
    {
        perror("error wait");
        return 9;
    }
    return 0;
}
