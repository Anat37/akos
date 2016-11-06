#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<signal.h>

key_t key;
int msgid;

typedef struct {
	int a, b;
	long result;
	short flag;
} mem_t;

struct msgbuf {
	long mes_type;
	mem_t str;
};

struct msgbuf* data;

void handler(int sig)
{
	if (sig == SIGWINCH)
	{
		msgctl(shmid, sizeof(mem_t), IPC_RMID);
		exit(0);
	}		
}

int main()
{
	key = ftok("/bin/ls", '1');
	msgid = msgget(key, sizeof(mem_t), IPC_CREAT | IPC_EXCL | 0600);
	if (msgid == (-1))
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
	signal(SIGWINCH, handler);
	
	while(1)
	{
		msgrcv(msgid, &data, sizeof(mem_t), 1, 0)
		data->result = data->a + data->b;
		
		data->mes_type = 2;
		msgsnd(msgid, &data, sizeof(mem_t), 0);
		
	}
	
	
		
	return 0;
}
