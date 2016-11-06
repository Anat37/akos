#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<syslog.h>
#include<pwd.h>
#include<signal.h>
#include<fcntl.h>
#include<unistd.h>

#define DAEMON_NAME "my_daemon"
#define DAEMON_USERNAME "kozlov"

void* handler(int sig)
{
	if (sig == SIGUSR1)
	{
		syslog(LOG_NOTICE, "SIGUSR1 received");
	}
	signal(SIGUSR1, handler);
}

int main(int argc, char* argv[])
{
	uid_t uid;
	pid_t pid;	
	int pid_file;
	FILE* pidf;
	int status;
	
	uid = getuid();
	
	if (!uid)
	{
		fprintf(stderr, "No root laws");
		return 2;
	}
	
	fd = open("/var/run/mydaemon", O_CREAT | O_WRONLY, 0644);
	if (pid_file == -1)
	{
		perror("error creating file");
		return 2;
	}
	if (fcntl(pid_file, F_SETLEASE, F_WRLCK) == -1)
	{
		perror("file blocked");
		return 2;
	}
	
	openlog(DAEMON_NAME, LOG_CONS, LOG_DAEMON);
	
	struct passwd *pass = getpwnam(DAEMON_USER);
	if (pass == NULL)
	{
		fprintf(stderr, "No user %s", DAEMON_USER);
		return 2;
	}
	
	uid = pass->pw_uid;
	seteuid(uid);
	
	pid = fork();
	
	if (pid != 0)
		return 0;
		
	if (setpgrp() == -1)
	{
		perror("changing group");
		return 2;
	}	
	if (setseid() == -1)
	{
		perror("set session");
		return 2;
	}
	
	
	close(0);
	close(1);
	close(2);
	
	pid = getpid();
	
	pidf = fdopen(pid_file, "w");
	if (fprintf(pidf, "%ld", (long)pid))
	{
		syslog(LOG_ERR, "write_error\n");
		return -1;	
	}
	fflush(pidf);
	
	signal(SIGUSR1, handler);
	
	while(1)
	{
		pause();
	}
	
	return 0;
}
