#include <pwd.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/file.h>
#include <sys/types.h>

#define DAEMON_NAME "my_awesome_daemon"
#define DAEMON_USERNAME "daemonuser"
#define NUMSEMS 2
#define SHMEMPATH "/bin/ls"
#define SHMEMKEY '1'

typedef struct {
    char *str;
} mem_t;

key_t key;
int shmid;
int semid;

mem_t* data;
struct sembuf sb[2] = {{0, 1, 0}, {0, 0, 0}};
struct sembuf wait_client[1] = {{0, -1, 0}};
struct sembuf calculated[1] = {{1, 1, 0}};

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

  if (flock(pid_file_fd, LOCK_EX | LOCK_NB) == -1)
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
  key = ftok(SHMEMPATH, SHMEMKEY);
  shmid = shmget(key, sizeof(mem_t), IPC_CREAT | 0644);
  if (shmid == -1)
  {
      perror("shared memory id get error");
      return 1;
  }

  semid = semget(key, NUMSEMS, IPC_CREAT | 0644);
  if (semid == -1)
  {
      printf("semaphors id get failed\n");
      return 1;
  }

  data = (mem_t*)shmat(shmid, NULL, 0);
  if (data == NULL)
  {
      printf("shmat failed\n");
      return 2;
  }

  while (1)
  {
      semop(semid, wait_client, 1);
      syslog(LOG_NOTICE, "%s", data->str);
      semop(semid, calculated, 1);
  }

  return 0;
}
