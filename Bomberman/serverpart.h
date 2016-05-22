#define MAGIC_CONST 5
#define THREADS_INIT 16
#define T_PLAYER 101
#define T_HOST 102
#define ST_WAITING 201
#define ST_READY 202
#define ST_PLAYING 203
#define MUTEX_CNT 5

int sock_id;
int bind_ret;
int acc_ret;
struct sockaddr_in server_addr;

struct splayer{
	int thread_id = 0;
	char* name = NULL;
};

struct sthread {
    int free;
    int desc;
    int type; 
	int id;
	int team_id;
	struct splayer* pl = NULL;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

struct steam {
	int max_pl_cnt = 0;
	int pl_cnt = 0;
	int status = ST_WAITING;
	int* pl = NULL;
	char* name = NULL;
	void* map = NULL;
	pthread_mutex_t mutex;
};

volatile int players_cnt;
volatile int teams_cnt;
volatile int team_arr_size = 0;
volatile int player_arr_size = 0;
volatile int connected;
volatile int first_free;
volatile int threadscnt = THREADS_INIT;
volatile struct steam* team_arr;
volatile struct splayer* player_arr; 
volatile truct sthread* sthreads;
pthread_t* plthread;

pthread_mutex_t mutex[MUTEX_CNT];
/* 0 -mutex for connected,
 * 1 - mutex for teams,
 * 2 - for team arr,
 * 3 - for player arr
 */


int stop_server()
{
	int i;
        close(sock_id);
        for (i = 0; i < threadscnt; ++i)
        {
        	pthread_cancel(plthread[i]);
        }
        for (i = 0; i < threadscnt; ++i)
        {
        	pthread_join(plthread[i], NULL);
        	pthread_cond_destroy(&(sthreads[i].cond));
        	pthread_mutex_destroy(&(sthreads[i].mutex));
        }
		for (i = 0; i < MUTEX_CNT; ++i)
        	pthread_mutex_destroy(&mutex[i]);
		free(team_arr);
		free(player_arr);
		free(sthreads);
		free(plthread);
        free_mem();
       	return 0;
}

int err_solve(size_t size, void* buf) /* ret val > 0 if error wasn't solved*/
{
	return 0;
}

int add_team(int fd)
{
	int type = 0;
	int ret = 0; 
	size_t size = 0;
	void* buf = NULL;
	pthread_mutex_lock(&mutex[2]);
	if (teams_cnt + 1 < team_arr_size)
	{
		int i;
		team_arr = realloc(team_arr, (team_arr_size + 16) * sizeof(struct steam) );
		for (i = team_arr_size; i < team_arr_size + 16; ++i)
			pthread_mutex_init(&team_arr[i].mutex);
		team_arr_size += 16;	
	}
	++teams_cnt;
	pthread_mutex_unlock(&mutex[2]);
	ret = msg_rec(fd, &type, &size, &buf);
	if (ret != 0)
		return ret;
	ret = (int) buf;
	team_arr[teams_cnt - 1].max_pl_cnt = ret;
	team_arr[teams_cnt - 1].pl = malloc(ret * sizeof(int));
	ret = msg_rec(fd, &type, &size, &buf);
	if (ret != 0)
		return ret;
	team_arr[teams_cnt - 1].name = (char*) buf;
		
}

int add_player(int fd, int team_id, int pl_id)
{
	int type = 0;
	int ret = 0; 
	size_t size = 0;
	void* buf = NULL;
	pthread_mutex_lock(&mutex[3]);
	if (players_cnt + 1 < player_arr_size)
	{
		player_arr = realloc(player_arr, (player_arr_size + 16) * sizeof(struct splayer) );
		player_arr_size += 16;	
	}
	++players_cnt;
	pthread_mutex_unlock(&mutex[3]);
	ret = msg_rec(fd, &type, &size, &buf);
	if (ret != 0)
		return ret;
	player_arr[players_cnt - 1].name = (char*) buf;
	team_arr[team_id].pl[pl_cnt] = pl_id;
}

int player_init(struct sthread* ptr)
{	
	int i = 0;
	int type = 0;
	int ret = 0;
	size_t size = 0;
	void* buf = NULL;
	int fd = ptr->desc;
	msg_send(fd, MSG_INFO_STRING, strlen(conn_succ) + 1, (void*)conn_succ);
	pthread_mutex_lock(&mutex[1]);
	msg_send(fd, MSG_INFO_NUM, sizeof(int), (void*)teams_cnt);
	for (i = 0; i < teams_cnt, ++i)
	{
		msg_send(fd, MSG_INFO_STRING, sizeof(team_arr[i].name) + 1, (void*) team_arr[i].name);
		msg_send(fd, MSG_INFO_NUM, sizeof(int), team_arr[i].status);
	}
	ret = msg_rec(fd, &type, &size, &buf);
	if (ret != 0)
	{
		pthread_mutex_unlock(&mutex[1]);
		return ret;
	}
	if ((int)*buf == -1)
	{
		ptr->type = T_HOST;
		ptr->team_id = teams_cnt;
		ret = add_team(fd);	
	} else 
	{
		ptr->type = T_PLAYER;
		ptr->team_id = (int) *buf;
		ret = add_player(fd, ptr->team_id,  ptr->id);
	}
	pthread_mutex_unlock(&mutex[1]);
	if (ret != 0)
		return ret;
	return 0; 
}

void player_gameplay()
{
}
void team_gameplay(){
	
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
        if (ptr->type = T_PLAYER)
			player_gameplay(ptr);
		else 
			team_gameplay(ptr);
	        
		close(ptr->desc);
		pthread_mutex_lock(&(ptr->mutex));
		pthread_mutex_lock(&(mutex[0]));
	
		--connected;
		if (ptr->id < first_free)
			first_free = ptr->id;
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
void add_threads(int cnt)
{
	sthreads = realloc(sthreads, sizeof(struct sthread) * (threadscnt + cnt));
	plthread = realloc(plthread, sizeof(pthread_t) * (threadscnt + cnt));
	     
   	for(i = threadcnt; i < threadscnt  + cnt; ++i)
   	{
       	pthread_mutex_init(&(sthreads[i].mutex), NULL);
       	pthread_cond_init(&(sthreads[i].cond), NULL);
       	pthread_create(&plthread[i], NULL, playerthread, &sthreads[i]); 
       	sthreads[i].free = 0;
   	}
	first_free = threadscnt;
	threadscnt +=cnt;
}

int server_work()
{
	int i = 0;
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
   	
	for (i = 0; i < MUTEX_CNT; ++i)
	  	pthread_mutex_init(&mutex[i], NULL);
	sthreads = malloc(sizeof(struct sthread) * threadscnt);
	plthread = malloc(sizeof(pthread_t) * threadscnt);
	     
   	for(i = 0; i < threadscnt; ++i)
   	{
       	pthread_mutex_init(&(sthreads[i].mutex), NULL);
       	pthread_cond_init(&(sthreads[i].cond), NULL);
       	pthread_create(&plthread[i], NULL, playerthread, &sthreads[i]); 
       	sthreads[i].free = 0;
   	}
	
		
		
   	while (1)
   	{
       	int all_not_free;
       	acc_ret = accept(sock_id, NULL, NULL);
		printf("ok1 %d\n", connected);
       	pthread_mutex_lock(&mutex[0]);
       	all_not_free = (connected == threadscnt);
       	pthread_mutex_unlock(&mutex[0]);
       
       	if (all_not_free)
       		add_threads(16);
       	
      	pthread_mutex_lock(&mutex[0]);
       	++connected;
       	pthread_mutex_unlock(&mutex[0]);
       	for (i = first_free; i < threadscnt; ++i)
       	{
       		pthread_mutex_lock(&(sthreads[i].mutex));
       		if (sthreads[i].free == 0)
       		{
           		sthreads[i].free = 1;
           		sthreads[i].desc = acc_ret;
				sthreads[i].id = i;
				first_free = i + 1;
           		pthread_mutex_unlock(&(sthreads[i].mutex));
           		pthread_cond_signal(&(sthreads[i].cond));
           		break;
			}
			pthread_mutex_unlock(&(sthreads[i].mutex));
		}
    } /* WHILE END */

    return 0;
}
