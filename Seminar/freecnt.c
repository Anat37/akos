#include<fcntl.h>
#include<unistd.h>

int get_free()
{
	int pipes[2];
	pid_t pid;
	int ret;
	long mem;
	FILE* ptr;
	
	pid = fork();
	ret = pipe(pipes);
	dup2(0, pipe[0]);
	if (pid == 0){
		dup2(1, pipe[1]);
		close(pipe[0]);
		execlp("free", "free", NULL);
	}
	close(pipe[1]);
	ptr = fopen(pipe[0], "r");
	if (ptr == NULL)
	{
		printf("Open error");
		exit(0);
	}
	fscanf(ptr, "%l", &mem);
	wait(pid);
	close(pipe[0]);
	return mem;
}


struct sigevent sevp;
timer_t timerid;
struct sigaction sigact;

void alrm_handler(int sig, siginfo_t *siginfo, void* ptr)
{
	system("free | grep Память: | awk '{print $4}' > /tmp/tempTimer");	
	      
}
int main()
{
	sevp.sigev_notify = SIGEV_SIGNAL;
	sevp.sigev_signo = RT_SIGMIN;
	
	if (timer_create(CLOCK_REALTIME, &sevp, &timerid) == -1)
	{
		perror("timer_create fail");
		return 1;
	}
	
	timerspec.it_interval.tv_sec = 0;
	timerspec.it_interval.tv_nsec = 5 * 1E8;
	
	timerspec.it_value = timerspec.it_interval;
	if (timer_settime(timerid, TIMER_ABSTIME, &timerspec, NULL) == -1)
	{
		printf("error");
		return 1;
	} 
	
	sigact.sa_handler = NULL;
	sigact.sa_sigaction = alrm_handler;
	sigact.sa_mask = 0;
	sigact.sa_flags = SA_SIGINFO;
	
	if (sigaction(RT_SIGMIN, &sigact, NULL) == -1)
	{
		printf("error");
		return 2;
	}
	
	while(1){
		pause();
	}
	
	return 0;
}
