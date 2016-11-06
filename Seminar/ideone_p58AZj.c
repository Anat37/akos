#include <pwd.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define DAEMON_NAME "my_awesome_daemon"
#define DAEMON_USERNAME "daemonuser"

pid_t pid;
int pid_file_fd;

void usr1_handler(int sign)
{
  if (sign == SIGUSR1)
  {
    syslog(LOG_NOTICE, "SIGUSR1 received");
  }
  signal(SIGUSR1, usr1_handler);
}


int main()
{
  struct flock fl;
  uid_t uid;
  FILE *pid_file;

  uid = geteuid();
  if (uid != 0)
  {
    fprintf(stderr, "Sorry, root is required\n");
    return -1;
  }

  pid_file_fd = open("/var/run/" DAEMON_NAME,
                     O_CREAT | O_WRONLY,
                     0644);

  if (pid_file_fd == -1)
  {
    perror("open error");
    return -1;
  }


  fl.l_type   = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK    */
  fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
  fl.l_start  = 0;        /* Offset from l_whence         */
  fl.l_len    = 0;        /* length, 0 = to EOF           */
  fl.l_pid    = getpid(); /* our PID                      */
  if (fcntl(pid_file_fd, F_SETLKW, &fl) == -1)
  {
    perror("block failed");
    return -1;
  }

  openlog(DAEMON_NAME, LOG_CONS, LOG_DAEMON);

  struct passwd *pass = getpwnam(DAEMON_USERNAME);
  if (pass == NULL)
  {
    fprintf(stderr, "No such user: %s\n", DAEMON_USERNAME);
    return -1;
  }

  uid = pass->pw_uid;
  seteuid(uid);
  if (setpgrp() == -1)
  {
    perror("changing gid error");
    return -1;
  }

  pid = fork();
  if (pid != 0) /* parent process: we should exit */
  {
    return 0;
  }

  /* now we are in child(daemon) process */
  if (setsid() == -1) /* creating new session so daemon isn't killed by stopping initial terminal process */
  {
    perror("creating session error");
    return -1;
  }

  pid = getpid();

  pid_file = fdopen(pid_file_fd, "w");
  if (pid_file == NULL)
  {
    syslog(LOG_ERR, "Could manage to open fd [%d]\n", pid_file_fd);
    return -1;
  }

  fprintf(pid_file, "%ld", (long)pid);
  fflush(pid_file);

  /* closing standart fd */
  close(0);
  close(1);
  close(2);

  /* now main code part starts */
  signal(SIGUSR1, usr1_handler);

  while (1)
  {
    pause();
  }

  return 0;
}
