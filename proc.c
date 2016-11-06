#include<unistd.h>
#include<stdio.h>
#include<signal.h>

volatile int pip[2];
volatile pid_t p, t1, t2, t3;
volatile int status;
short flag = 1;

int handler(int sig)
{
	if (sig == SIGUSR1)
	{
		read(pip[0], (void *)&t2, sizeof(pid_t));
		read(pip[0], (void *)&t3, sizeof(pid_t));
		write(pip[1], (void *)&t3, sizeof(pid_t));
		kill(t2, SIGUSR2);
		printf("parent = %ld, *son1 = %ld, son2 = %ld, son3 = %ld\n", p, t1, t2, t3);
		close(pip[0]);
		close(pip[1]); 
		exit(0);
	}
	
	if (sig == SIGUSR2)
	{
		read(pip[0], (void *)&t3, sizeof(pid_t));
		printf("parent = %ld, son1 = %ld, *son2 = %ld, son3 = %ld\n", p, t1, t2, t3); 
		close(pip[0]);
		close(pip[1]);
		exit(0);
	}
	return 0;
}

int main()
{
	
	
	p = getpid();
	
	if (pipe(pip))
	{
		return -1;
	}
	
	t1 = fork();
	if (t1 == -1)
		return -1; 
	if (t1 == 0)
	{
		t1 = getpid();
		signal(SIGUSR1, &handler);
		while (flag)
		{
			pause();
		}
		return 0;
	}
	
	t2 = fork();
	if (t2 == -1)
		return -1; 
	if (t2 == 0)
	{
		t2 = getpid();
		signal(SIGUSR2, &handler);
		while (flag)
		{
			pause();
		}
		return 0;
	}
	
	t3 = fork();
	if (t3 == -1)
		return -1; 
	if (t3 == 0)
	{	
		close(pip[0]);
		close(pip[1]);
		t3 = getpid();
		printf("parent = %ld, son1 = %ld, son2 = %ld, *son3 = %ld\n", p, t1, t2, t3); 
		return 0;
	}
	
	write(pip[1],(void *)&t2, sizeof(pid_t));
	write(pip[1],(void *)&t3, sizeof(pid_t));
	kill(t1, SIGUSR1);
	
	close(pip[0]);
	close(pip[1]);
	waitpid(t1, &status, 0);
	waitpid(t2, &status, 0);
	waitpid(t3, &status, 0);
	printf("*parent = %ld, son1 = %ld, son2 = %ld, son3 = %ld\n", p, t1, t2, t3); 
	
	return 0;
}
