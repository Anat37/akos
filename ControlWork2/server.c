#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <netinet/ip.h>
#include <netinet/in.h>

#define MAGIC_CONST 5
#define THREADS 10

int sock_id;
int port_num;
int bind_ret;
int acc_ret;
struct sockaddr_in server_addr;

struct sthread {
    int free;
    int desc;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

volatile int numconnected;
struct sthread arr[THREADS];
pthread_t arrthread[THREADS];
int i;

pthread_mutex_t mutex[1];


int msg_rec(int desc, char** arg)
{
	int readed = 0;
	int read_ret = 0;
	char* str;
	str = malloc(25);
	
	do {
		read_ret = read(desc, str + readed, sizeof(char));
		readed += read_ret;
		if (read_ret == 0) return 0;
		if (read_ret == -1) return -1;
		if (readed > 20) return -1; 
	} while (*(str + readed - 1) != '\0');
	*arg = str;
	return 0; 
}

void msg_send(int desc, size_t len, const char* arg)
{
	write(desc, arg, len);
	return; 
}

const char msgok[] = "hello";
const char* msg10 = "SEVEN YES";
const char* msg2 = "SEVEN NO";
const char* msg31 = "LARGE NUMBER 1";
const char* msg32 = "LARGE NUMBER 2";
const char* msg4 = "WORK END";
void handler(int sig)
{
    if (sig == SIGTERM)
    {
        int i;
        close(sock_id);
        for (i = 0; i < THREADS; ++i)
        {	
            msg_send(arr[i].desc, strlen(msg4) + 1, msg4);
            pthread_cancel(arrthread[i]);
        }
        for (i = 0; i < THREADS; ++i)
        {
            pthread_join(arrthread[i], NULL);
            pthread_cond_destroy(&(arr[i].cond));
            pthread_mutex_destroy(&(arr[i].mutex));
        }
        pthread_mutex_destroy(&mutex[0]);
        exit(0);
    }
}


void* fthread(void* arg)
{
	char* str1;
	char* str2;
	int len;
	int ret;
	int a,b;
	struct sthread* ptr = (struct sthread*) arg;
	while (1)
	{
		pthread_mutex_lock(&(ptr->mutex));
		pthread_cond_wait(&(ptr->cond), &(ptr->mutex)); 
		ptr->free = 0;
		pthread_mutex_unlock(&(ptr->mutex));
		
		if (msg_rec(ptr->desc, &str1) < 0)
			msg_send(ptr->desc, strlen(msg31) + 1, msg31);
		else {
			if (msg_rec(ptr->desc, &str2) <0)
				msg_send(ptr->desc, strlen(msg32) + 1, msg32);
			else {
					a = atoi(str1);
					b = atoi(str2);
					if (a < 0) 
					{
						msg_send(ptr->desc, strlen(msg31) + 1, msg31);
					} else if (b < 0)
						{
							msg_send(ptr->desc, strlen(msg32) + 1, msg32);
						} else 
							if (a + b % 7 == 0)
							{
								msg_send(ptr->desc, strlen(msg10) + 1, msg10);
							} else 
								msg_send(ptr->desc, strlen(msg2) + 1, msg2);
				}
		}
		close(ptr->desc);
		free(str1);
		free(str2);

		pthread_mutex_lock(&(ptr->mutex));
		pthread_mutex_lock(&(mutex[0]));
		--numconnected;
		ptr->free = 1;
		pthread_mutex_unlock(&(mutex[0]));
		pthread_mutex_unlock(&(ptr->mutex));
	}
return NULL;
}

const char msg1[] = "error";

int main(int argc, char** argv)
{
    numconnected = 0;
    
    if (argc != 2)
    {
        printf("fail argc\n");
        return -1;
    }
    port_num = atoi(argv[1]);
    sock_id = socket(PF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((short)port_num);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind_ret = bind(sock_id, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr_in));
    if (bind_ret == -1)
    {
        printf("fail bind\n");
        return -2;
    }
    listen(sock_id, MAGIC_CONST);
    signal(SIGTERM, handler);
   
    for(i = 0; i < THREADS; ++i)
    {
        pthread_mutex_init(&(arr[i].mutex), NULL);
        pthread_cond_init(&(arr[i].cond), NULL);
    }

    for(i = 0; i < THREADS; ++i)
    {
        pthread_create(&arrthread[i], NULL, fthread, &arr[i]); 
    }

    pthread_mutex_init(&mutex[0], NULL);

    while (1)
    {
        int all_not_free;
        acc_ret = accept(sock_id, NULL, NULL);

        pthread_mutex_lock(&mutex[0]);
        all_not_free = (numconnected == THREADS);
        pthread_mutex_unlock(&mutex[0]);
        
        if (all_not_free)
        {
            write(acc_ret, msg1, strlen(msg1) + 1);  
            continue;
        }

        pthread_mutex_lock(&mutex[0]);
        ++numconnected;
        pthread_mutex_unlock(&mutex[0]);

        for (i = 0; i < THREADS; ++i)
        {
            pthread_mutex_lock(&(arr[i].mutex));
            if (arr[i].free == 1)
            {
                arr[i].free = 2;
                arr[i].desc = acc_ret;
                pthread_cond_signal(&arr[i].cond);
            	pthread_mutex_unlock(&(arr[i].mutex));
                break;
            }
            pthread_mutex_unlock(&(arr[i].mutex));
        }
    } /* WHILE END */

    return 0;
}
