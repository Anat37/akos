#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<signal.h>

#define NUMSEMS 4
#define RES_RELEASE 2

key_t key;

int shmid, semid;

typedef struct {
	int a, b;
	long result;
	short flag;
} mem_t;

struct sembuf use_resource[1] =  {{RES_RELEASE, -1, 0}};
struct sembuf free_res[1] = {{RES_RELEASE, 1, 0}};
struct sembuf wake_up[1] = {{0, 1, 0}};
struct sembuf wait_server[1] = {{1, -1, 0}};

mem_t* data;

int main()
{
	int a, b;
	long result;

	key = ftok("/bin/ls", '1');
	shmid = shmget(key, sizeof(mem_t), 0);
	semid = semget(key, NUMSEMS, 0);
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
	while (!feof(stdin)){
		scanf("%d %d\n", &a, &b);
	
		semop(semid, use_resource, 1);
		
		data->a = a;
		data->b = b;
		
		semop(semid, wake_up, 1);
		semop(semid, wait_server, 1);
		
		result = data->result;
		
		semop(semid, free_res, 1);
		printf("%ld\n", result);
		fflush(stdout);
	}
	shmdt(data);
	return 0;
}
