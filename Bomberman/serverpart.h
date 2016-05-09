#define MAGIC_CONST 5
#define MAX_PLAYERS 16
#define MAX_TEAMS 4
#define PLAYERN 101
#define HOSTN 102

int sock_id;
int bind_ret;
int acc_ret;
struct sockaddr_in server_addr;

struct sthread {
    	int free;
    	int desc;
    	int type; 
    	pthread_mutex_t mutex;
    	pthread_cond_t cond;
};

volatile int players_cnt;
volatile int teams_cnt;
volatile int connected;
struct sthread sthreads[MAX_PLAYERS + MAX_TEAMS];
pthread_t plthread[MAX_PLAYERS + MAX_TEAMS];
int i;

pthread_mutex_t mutex[1];



int stop_server()
{
	int i;
        close(sock_id);
        for (i = 0; i < MAX_PLAYERS + MAX_TEAMS; ++i)
        {
        	pthread_cancel(plthread[i]);
        }
        for (i = 0; i < MAX_PLAYERS + MAX_TEAMS; ++i)
        {
        	pthread_join(plthread[i], NULL);
        	pthread_cond_destroy(&(sthreads[i].cond));
        	pthread_mutex_destroy(&(sthreads[i].mutex));
        }
        pthread_mutex_destroy(&mutex[0]);
        free_mem();
       	return 0;
}



void player_init(struct sthread* ptr)
{	
	int fd = ptr->desc;
	msg_send(fd, MSG_INFO_STRING, strlen(conn_succ) + 1, (void*)conn_succ);
	msg_send(fd, MSG_INFO_NUM, sizeof(int), (void*)teams_cnt);
	 
}

void player_gameplay()
{
}

void* playerthread(void* arg)
{	
	struct sthread* ptr = (struct sthread*) arg;
	while (1)
	{
		pthread_mutex_lock(&(ptr->mutex));
		pthread_cond_wait(&(ptr->cond), &(ptr->mutex)); 
        	pthread_mutex_unlock(&(ptr->mutex));
        
		player_init(ptr);
        
		player_gameplay();
	        
		close(ptr->desc);
		pthread_mutex_lock(&(ptr->mutex));
		pthread_mutex_lock(&(mutex[0]));
	
		--connected;
		ptr->free = 0;

		pthread_mutex_unlock(&(mutex[0]));
		pthread_mutex_unlock(&(ptr->mutex));
	}
	return NULL;
}
void handler(int sig)
{
	stop_server();
	exit(0);
}


int server_work()
{
	connected = 0;
    	
    	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((short)port_num);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bind_ret = bind(sock_id, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr_in));
	if (bind_ret == -1)
	{
		err_report("Fail to bind", 0);
        	return 2;
    	}
    	signal(SIGINT, &handler);
    	listen(sock_id, MAGIC_CONST);
    	  
    	for(i = 0; i < MAX_PLAYERS + MAX_TEAMS; ++i)
    	{
        	pthread_mutex_init(&(sthreads[i].mutex), NULL);
        	pthread_cond_init(&(sthreads[i].cond), NULL);
        	pthread_create(&plthread[i], NULL, playerthread, &sthreads[i]); 
        	sthreads[i].free = 0;
    	}

    	pthread_mutex_init(&mutex[0], NULL);

    	while (1)
    	{
        	int all_not_free;
        	acc_ret = accept(sock_id, NULL, NULL);
		printf("ok1 %d\n", connected);
        	pthread_mutex_lock(&mutex[0]);
        	all_not_free = (connected == MAX_PLAYERS + MAX_TEAMS);
        	pthread_mutex_unlock(&mutex[0]);
        
        	if (all_not_free)
        	{
            		msg_send(acc_ret, 1, sizeof(msg1) + 1, msg1);  
            		continue;
        	}

        	pthread_mutex_lock(&mutex[0]);
        	++connected;
        	pthread_mutex_unlock(&mutex[0]);

        	for (i = 0; i < MAX_PLAYERS + MAX_TEAMS; ++i)
        	{
            		pthread_mutex_lock(&(sthreads[i].mutex));
            		if (sthreads[i].free == 0)
            		{
                		sthreads[i].free = 1;
                		sthreads[i].desc = acc_ret;
                		pthread_mutex_unlock(&(sthreads[i].mutex));
                		pthread_cond_signal(&(sthreads[i].cond));
                		break;
            		}
            		pthread_mutex_unlock(&(sthreads[i].mutex));
        	}
    } /* WHILE END */

    return 0;
}
