#include<signal.h>
#include<sys/types.h>
#include<stdio.h>

int main()
{
	int pid;
	FILE* fstr = fopen("/var/run/bomberserver", "r");
	
	if (fstr == NULL)
	{
		printf("No such a file\n");
		return 0;
	}
	fscanf(fstr, "%d", &pid);
	kill((pid_t)pid, SIGUSR1);
	return 0;
}
