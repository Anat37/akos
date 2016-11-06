#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>

#define MAGIC_CONST 5
#define THREADS 4

volatile int sockid;

volatile int bind_ret;
volatile int acc_ret;
volatile int cnt;
char* host_name;

int port_id;
const char msgok[]="hello";

struct hostent* hst;
struct sockaddr_in server_addr;

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

int main(int argc, char* argv[])
{
	int i = 0;
	sockid = socket(PF_INET, SOCK_STREAM, 0);
	
	port_id = atoi(argv[1]);
	host_name = argv[2];
	
	hst = gethostbyname(hostname);
	
	
	
	
	return 0;
}
