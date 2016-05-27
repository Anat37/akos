

int sock_id;
int bind_ret;
int acc_ret;
struct sockaddr_in server_addr;


volatile int players_cnt = 0;
volatile int teams_cnt = 0;
volatile int team_arr_size = 0;
volatile int player_arr_size = 0;



volatile int connected;
volatile int first_free = 0;
volatile int threadscnt = THREADS_INIT;


/* 0 -mutex for connected,
 * 1 - mutex for teams,
 * 2 - for team arr,
 * 3 - for player arr
 */


int stop_server()
{
	int i,j;
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
    for (i = 0; i < teams_cnt; ++i){
      pthread_mutex_destroy(&team_arr[i].mutex);
      free(team_arr[i].pl);
      free(team_arr[i].name);
      if (team_arr[i].map != NULL){
        for (j = 0; j < map_size_n; ++j)
          free(team_arr[i].map[j]);
      }
      free(team_arr[i].map);
      if (team_arr[i].objects != NULL)
        for (j = 0; j < map_size_n; ++j)
          free(team_arr[i].objects[j]);
      free(team_arr[i].objects);
      if (team_arr[i].map_mutex != NULL)
        for (j = 0; j < map_size_n; ++j)
          pthread_mutex_destroy(&team_arr[i].map_mutex[j]);
      free(team_arr[i].map_mutex);
    }
    for (i = 0; i < players_cnt; ++i){
      free(player_arr[i].name);
    }
		free(team_arr);
		free(player_arr);
		free(sthreads);
		free(plthread);
    free_mem();
    return 0;
}






int player_init(struct sthread* ptr)
{	
	int i = 0;
	int type = 0;
	int ret = 0;
	size_t size = 0;
	void* buf = NULL;
	int fd = ptr->desc;
  printf("Start init\n");
	msg_send(fd, MSG_INFO_STRING, strlen(conn_succ) + 1, (void*)conn_succ);
	
  pthread_mutex_lock(&mutex[1]);
	msg_send(fd, MSG_INFO_NUM, sizeof(int), (void*)&teams_cnt);
  
	for (i = 0; i < teams_cnt; ++i)
	{
		msg_send(fd, MSG_INFO_STRING, strlen(team_arr[i].name) + 1, (void*) team_arr[i].name);
		msg_send(fd, MSG_INFO_NUM, sizeof(int), &team_arr[i].status);
	}
	ret = msg_rec(fd, &type, &size, &buf);
	if (ret != 0)
	{
		pthread_mutex_unlock(&mutex[1]);
		return ret;
	}
	if (*(int*)buf == -1)
	{
		ptr->type = T_HOST;
		ptr->team_id = teams_cnt;
		ret = add_team(fd, ptr->id);	
	} else 
	{
		ptr->type = T_PLAYER;
		ptr->team_id = *(int*)buf;
    ptr->pl = players_cnt;
		ret = add_player(fd, ptr->team_id);
	}
	pthread_mutex_unlock(&mutex[1]);
	return ret; 
}


#include "gameplay.h"

void* playerthread(void* arg)
{	
	struct sthread* ptr = (struct sthread*) arg;
	while (1)
	{
		pthread_mutex_lock(&(ptr->mutex));
		pthread_cond_wait(&(ptr->cond), &(ptr->mutex)); 
    pthread_mutex_unlock(&(ptr->mutex));
		player_init(ptr);
    printf("end init");
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
  printf("SIGINT\n");
	exit(0);
}


void restart_thr(int k)
{
  pthread_t id;
  disconnect(&sthreads[k]);  
  id = pthread_self();
  if (plthread[k] == id)
    sthreads[k].free = -1;
  else
    sthreads[k].free = 0;
  --connected;
	if (k < first_free)
		first_free = k;
  printf("Client %d disconnected\n", k);
  pthread_cancel(plthread[k]);
  pthread_join(plthread[k], NULL);
  pthread_create(&plthread[k], NULL, playerthread, &sthreads[k]);
}




void handup(int sig)
{
  int poll_ret;
  int i;
  struct pollfd polls;
  printf("SIGPIPE\n");
  signal(SIGPIPE, &handup);
  polls.fd = 0;
	polls.events = POLLERR | POLLHUP;
	polls.revents = 0;
	for (i = 0; i < threadscnt; ++i)
  {
    if (sthreads[i].free == 0)
      continue;
    polls.fd = sthreads[i].desc;
    poll_ret = poll(&polls, 1, 0);
    if (poll_ret == -1)
    {
      err_report("poll failed\n", 1);
      handler(SIGINT);
    }
    if (polls.revents & POLLERR) {
      err_report("poll returned POLLERR\n", 0);
      handler(SIGINT);
    }
    if (polls.revents & POLLHUP) {
      restart_thr(i);
    }
  }
}



void add_threads(int cnt)
{
	int i;
	sthreads = realloc(sthreads, sizeof(struct sthread) * (threadscnt + cnt));
	plthread = realloc(plthread, sizeof(pthread_t) * (threadscnt + cnt));
	     
   	for(i = threadscnt; i < threadscnt  + cnt; ++i)
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
  printf("Switch to server\n");
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
    signal(SIGPIPE, &handup);
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
	
		
		printf("listening\n");
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
          if (sthreads[i].free == -1)
          {
              pthread_create(&plthread[i], NULL, playerthread, &sthreads[i]); 
              pthread_mutex_unlock(&(sthreads[i].mutex));
              sleep(1);
              pthread_mutex_lock(&(sthreads[i].mutex));
              sthreads[i].free = 0;
          }
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
