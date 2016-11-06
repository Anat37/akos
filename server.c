#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<signal.h>

#define NUMSEMS 4
#define RES_RELEASE 2

key_t key;
int shmid,semid;

typedef struct {
	int a, b;
	long result;
	short flag;
} mem_t;

struct sembuf res_release[1] = {{RES_RELEASE, 1, 0}};
struct sembuf wait_client[1] = {{0, -1, 0}};
struct sembuf client_sem[1] = {{1, 1, 0}};
mem_t* data;

void handler(int sig)
{
	if (sig == SIGUSR1)
	{
		shmdt(data);
		semctl(semid, NUMSEMS, IPC_RMID);
		shmctl(shmid, sizeof(mem_t), IPC_RMID);
		exit(0);
	}		
}

int main()
{
	key = ftok("/bin/ls", '1');
	shmid = shmget(key, sizeof(mem_t), IPC_CREAT | IPC_EXCL | 0600);
	semid = semget(key, NUMSEMS, IPC_CREAT | IPC_EXCL | 0600);
	if (shmid == (-1) || semid == (-1))
	{
		printf("ipc get error\n");
		return 1;
	}
	
	data = (mem_t*) shmat(shmid, NULL, 0);
	if ((void*)(-1) == (void*)data)
	{
		printf("shmat error\n");
		return 1;
	}
	signal(SIGUSR1, handler);
	semop(semid, res_release, 1);
	
	while (1)
	{
		semop(semid, wait_client, 1);
		data->result = data->a + data->b;
		semop(semid, client_sem, 1);		 	
	}	
		
	return 0;
}
