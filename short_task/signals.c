#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int k = 0;

void hndl(int sig, siginfo_t* inf, void* ptr)
{
	printf("%d\n", (*inf).si_pid);
	k++;
	return;
}

int main()
{
    	struct sigaction act;
	sigset_t   set;
    	int fl = 0;
    	int i = 0;
    	
    	fl = fl | SA_SIGINFO;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = hndl; 
	sigfillset(&set);
	act.sa_flags = fl;                                            
	act.sa_mask = set;
	for (i; i < 32; ++i)	
	sigaction(i, &act, 0);
	
	while(k < 10){};	
	
    return 0;
}

