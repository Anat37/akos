
#define DAEMON_NAME "bomberserver"
#define DAEMON_USERNAME "daemonuser"

char* logname = "syslog";
char* pid_file_name = DAEMON_NAME; 
int pid_file_fd;

int stop_server();

void usr1_handler(int sig)
{	
	syslog(LOG_NOTICE, "Server shut down by SIGUSR1");
	closelog();
	close(pid_file_fd);
	exit(0);
}

int become_daemon()
{
	pid_t pid;
	struct flock fl;
  	uid_t uid;
  	FILE *pid_file;
  	struct passwd *pass = getpwnam(DAEMON_USERNAME);

  	uid = geteuid();
  	if (uid != 0)
  	{
    		fprintf(stderr, "Sorry, root is required for being daemon\n");
    		return 1;
  	}

  	pid_file_fd = open("/var/run/DAEMON_NAME", O_CREAT | O_WRONLY, 0644);

  	if (pid_file_fd == -1)
  	{
    		perror("Open pid_file error");
    		return 1;
  	}


  	fl.l_type   = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK    */
  	fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
  	fl.l_start  = 0;        /* Offset from l_whence         */
  	fl.l_len    = 0;        /* length, 0 = to EOF           */
  	fl.l_pid    = getpid(); /* our PID                      */
  	if (fcntl(pid_file_fd, F_SETLKW, &fl) == -1)
  	{
    		perror("block failed");
    		return 1;
  	}

  	openlog(logname, LOG_CONS, LOG_DAEMON);

  	if (pass == NULL)
  	{
    		fprintf(stderr, "No such user: %s\n", DAEMON_USERNAME);
    		return 1;
  	}
  
  	uid = pass->pw_uid;
  	seteuid(uid);
  	if (setpgrp() == -1)
  	{
    		perror("Changing gid error");
    		return 1;
  	}

  	pid = fork();
  	if (pid != 0) /* parent process: we should exit */
  	{
    		exit(0);
  	}

  	/* now we are in child(daemon) process */
  	if (setsid() == -1) /* creating new session so daemon isn't killed by stopping initial terminal process */
  	{
    		perror("Creating session error. Unable to proceed");
    		closelog();
    		close(pid_file_fd);
    		exit(0);
  	}
	
	/* closing standart fd */
  	close(0);
  	close(1);
  	close(2);
  
  	pid = getpid();

  	pid_file = fdopen(pid_file_fd, "w");
  	if (pid_file == NULL)
  	{
    		syslog(LOG_ERR, "Could manage to open pid file fd [%d]\n", pid_file_fd);
    		closelog();
    		close(pid_file_fd);
    		exit(0);
  	}

  	fprintf(pid_file, "%ld", (long)pid);
  	fflush(pid_file);
	signal(SIGUSR1, &usr1_handler);
	return 0;
}
