#include<signal.h>
#include<stdio.h>
#include<unistd.h>

int pipe[2];
pid_t pid[6];
volatile short flag = 1;

int handlergs1(int sig)
{
	if (sig == SIGUSR1)
	{
	}
	if (sig == SIGUSR2)
	{
	}
	return 0;
}

int handlergs2(int sig)
{
	if (sig == SIGUSR1)
	{
	}
	if (sig == SIGUSR2)
	{
	}
	return 0;
}

int handlers2(int sig)
{
	if (sig == SIGUSR1)
	{
	}
	if (sig == SIGUSR2)
	{
	}
	return 0;
}

int handlers3(int sig)
{
	if (sig == SIGUSR1)
	{
	}
	if (sig == SIGUSR2)
	{
	}
	return 0;
}

int main()
{
	
	
	pid[0] = getpid();
	pid[1] = fork();
	if (pid[1] == 0)
	{
		pid[1] = getpid();
		pid[2] = fork();
		if (pid[2] == 0)
		{
			pid[2] = getpid();
			/*grandson 1, know 0,1,2*/
			while (flag)
			{
				pause();
			}
		}
		pid[3] = fork();
		if (pid[3] == 0)
		{
			pid[3] = getpid();
			/*grandson 2, know 0,1,2,3*/
			while (flag)
			{
				pause();
			}
		}
		/*son 1, know 0,1,2,3*/
		while (flag)
		{
			pause();
		}
	}
	pid[4] = fork();
	if (pid[4] == 0)
	{
		pid[4] = getpid();
		/*son 2, know 0,1,4*/
		while (flag)
		{
			pause();
		}
	}
	pid[5] = fork();
	if (pid[5] == 0)
	{
		pid[5] = getpid();
		/*son 3, know 0,1,4,5*/
		while (flag)
		{
			pause();
		}
	}
	/*parent, know 0,1,4,5*/
	
	return 0;
}

