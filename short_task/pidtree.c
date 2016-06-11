#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int pipes[2];
pid_t pid[6];
pid_t pidc[6];
volatile short flag = 1;

int wr()
{
	write(pipes[1], 6 * sizeof(pid_t), pid);
	return 0;
}

int rd()
{
	read(pipes[0], 6 * sizeof(pid_t), pidc);
	return 0;	
}

int handlerfors1(int sig)
{
	if (sig == SIGUSR1)
	{
		rd();
		pid[2] = pidc[2];
		pid[3] = pidc[3];
		flag = 0;
	}
	return 0;
}

int handlerforall(int sig)
{
	if (sig == SIGUSR1)
	{
		rd();
		flag = 0;
	}
	return 0;
}

int handlers1(int sig)
{
	rd();
	pidc[2] = pid[2];
	pidc[3] = pid[3];
	wr();
	kill(SIGUSR1, pid[0]);
	printf("Parent: %d, Me - Son1: %d, Son2: %d, Son3: %d, GrandSon1: %d, GrandSon2: %d\n",
		pidc[0], pidc[1], pidc[2], pidc[3], pidc[4], pidc[5]);
	close(pipes[0]);
	close(pipes[1]);
	flag = 0;
	return 0;
}


int handlergs1(int sig)
{
	rd();
	kill(SIGUSR1, pid[0]);
	printf("Parent: %d, Son1: %d, Son2: %d, Son3: %d, Me - GrandSon1: %d, GrandSon2: %d\n",
		pidc[0], pidc[1], pidc[2], pidc[3], pidc[4], pidc[5]);
	close(pipes[0]);
	close(pipes[1]);
	exit(0);
}

int handlergs2(int sig)
{
	rd();
	kill(SIGUSR1, pid[0]);
	printf("Parent: %d, Son1: %d, Son2: %d, Son3: %d, GrandSon1: %d, Me - GrandSon2: %d\n",
		pidc[0], pidc[1], pidc[2], pidc[3], pidc[4], pidc[5]);
	close(pipes[0]);
	close(pipes[1]);
	exit(0);
}

int handlers3(int sig)
{
	rd();
	kill(SIGUSR1, pid[0]);
	printf("Parent: %d, Son1: %d, Son2: %d, Me - Son3: %d, GrandSon1: %d, GrandSon2: %d\n",
		pidc[0], pidc[1], pidc[2], pidc[3], pidc[4], pidc[5]);
	close(pipes[0]);
	close(pipes[1]);
	exit(0);
}

int handlers2(int sig)
{
	rd();
	kill(SIGUSR1, pid[0]);
	printf("Parent: %d, Son1: %d, Me - Son2: %d, Son3: %d, GrandSon1: %d, Me - GrandSon2: %d\n",
		pidc[0], pidc[1], pidc[2], pidc[3], pidc[4], pidc[5]);
	close(pipes[0]);
	close(pipes[1]);
	exit(0);
}

int main()
{
	int i;
	int status = 0;
	struct sigaction sighand;
  	sigset_t   set; 
  	sigemptyset(&set); 
  	sighand.sa_sigaction = NULL;
	sighand.sa_mask = set;
	sighand.sa_flags = 0;
	
	if (pipe(pipes) == -1)
	{
		perror("Error creating pipe");
		return 0;
	}
	
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
			signal(SIGUSR1, &handlergs1);
			while (flag)
			{
				pause();
			}
			return 0;
		}
		pid[3] = fork();
		if (pid[3] == 0)
		{
			pid[3] = getpid();
			/*grandson 2, know 0,1,2,3*/
			signal(SIGUSR1, &handlergs2);
			while (flag)
			{
				pause();
			}
			return 0;
		}
		/*son 1, know 0,1,2,3*/
		signal(SIGUSR1, &handlers1);
		while (flag)
		{
			pause();
		}
		waitpid(pid[2], &status, 0);
		if (!WIFEXITED(status))
		{
			printf("Error finishing first grandson");
			return 1;
		}
		waitpid(pid[3], &status, 0);
		if (!WIFEXITED(status))
		{
			printf("Error finishing second grandson");
			return 1;
		}
		return 0;
	}
	pid[4] = fork();
	if (pid[4] == 0)
	{
		pid[4] = getpid();
		/*son 2, know 0,1,4*/
		signal(SIGUSR1, &handlers2);
		while (flag)
		{
			pause();
		}
		return 0;
	}
	pid[5] = fork();
	if (pid[5] == 0)
	{
		pid[5] = getpid();
		/*son 3, know 0,1,4,5*/
		signal(SIGUSR1, &handlers3);
		while (flag)
		{
			pause();
		}
		return 0;
	}
	/*parent, know 0,1,4,5*/
	
	wr();
	signal(SIGUSR1, &handlerfors1);
	kill(SIGUSR1, pid[1]);
	while (flag)
	{
		pause();
	}
	printf("Me - Parent: %d, Son1: %d, Son2: %d, Son3: %d, GrandSon1: %d, GrandSon2: %d\n",
		pid[0], pid[1], pid[2], pid[3], pid[4], pid[5]);
	
	
	for (i = 2; i < 6; ++i)
	{
		flag = 1;
		wr();
		signal(SIGUSR1, &handlerforall);
		kill(SIGUSR1, pid[i]);
		while (flag)
		{
			pause();
		}
	}
	
	close(pipes[0]);
	close(pipes[1]);
	waitpid(pid[1], &status, 0);
	if (!WIFEXITED(status))
	{
		printf("Error finishing first son");
		return 1;
	}
	waitpid(pid[4], &status, 0);
	if (!WIFEXITED(status))
	{
		printf("Error finishing second son");
		return 1;
	} 
	waitpid(pid[5], &status, 0);
	if (!WIFEXITED(status))
	{
		printf("Error finishing third son");
		return 1;
	}  
	return 0;
}


/*
	struct sigaction sigint;
  sigset_t   set; 
  sigemptyset(&set); 
  forpipe.sa_sigaction = NULL;
  forpipe.sa_handler = &handup;
	forpipe.sa_mask = set;
	forpipe.sa_flags = 0;
  
  sigaction(SIGPIPE, &forpipe, NULL);
  
  */