#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>

#define MAGIC_CONST 5
#define THREADS 4

volatile int sockid;
volatile int port_num;
volatile int bind_ret;
volatile int acc_ret;
volatile int cnt;
const char msgok[]="hello";

struct sockaddr_in server_addr;

struct sthread
{
	int free;
	int desc;
	pthread_mutex_t mutex;
	pthread_cond_t cond
	
} arr[THREADS];

int handler(int sig)
{
	if (sig == SIGINT)
	{
		close(sock_id);
		for (i = 0; i < THREADS; ++i)
		{
			pthread_cancel(arrthread[i]);
			pthread_join(arrthread[i]);
			pthread_mutex_destroy(&(arr[i].mutex));
			pthread_cond_destroy(&(arr[i].cond));
		}
		pthread_mutex_destroy(&mutex[0]);
		exit(0);
	}
}

void* fthread(void* arg)
{
	struct thread* ptr = (struct thread*) arg;
	while(1)
	{
		pthread_mutex_lock(&(ptr->mutex));
		pthread_cond_wait(&(ptr->cond), &(ptr->mutex));
		ptr->free = 0;
		++cnt;
		pthread_mutex_unlock(&(ptr->mutex));
		
		write(ptr->desc, msgok, strlen(msgok) + 1);
		close(ptr->desc);
		
		pthread_mutex_lock(&(ptr->mutex));
		pthread_mutex_lock(&(mutex[0]));
		
		--cnt;
		ptr->free = 1;
		
		pthread_mutex_unlock(&(ptr->mutex));
		pthread_mutex_unlock(&(mutex[0]));
	}
}

int main(int argc, char* argv[])
{
	int i = 0;
	sockid = socket(PF_INET, SOCK_STREAM, 0);
	
	port_num = atoi(argv[1]);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((short) port_num);
	server_addr.sin_addr = INADDR_ANY;
	bind_ret = bind(sock_id, (struct sockaddr*) &server_addr, sizeof(server_addr));
	if (bind_ret == -1)
	{
		perror("fail bind");
		return 1;
	}
	
	listen(sock_id, MAGIC_CONST);
	signal(SIGINT, handler);
	
	
	for (i; i < THREADS; ++i)
	{
		pthread_mutex_init(&(arr[i].mutex), NULL);
		pthread_cond_init(&(arr[i].cond), NULL);
		pthread_create(&artthread[i], NULL, fthread, NULL);
	}
	
	
	
	while(1)
	{
		int allfree;
		
		allfree = (cnt == THREADS);
		acc_ret = accept(, NULL, NULL);
		for (i = 0; arr[i].free	
	}
	
	for (i = 0; i < THREADS; ++i)
	{
		pthread_cancel(arrthread[i]);
		pthread_mutex_destroy(&(arr[i].mutex));
		pthread_cond_destroy(&(arr[i].cond));
	}
	return 0;
}











