#define MAGIC_CONST 5
#define MAX_PLAYERS 16
#define MAX_TEAMS 4

int sock_id;
int bind_ret;
int acc_ret;
struct sockaddr_in server_addr;

struct sthread {
    int free;
    int desc;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

volatile int players_cnt;
volatile int teams_cnt;
struct sthread players[MAX_PLAYERS];
struct sthread teams[MAX_TEAMS];
pthread_t plthread[MAX_PLAYERS];
pthread_t tthread[MAX_TEAMS];
int i;

pthread_mutex_t mutex[1];

int stop_server()
{
	int i;
        close(sock_id);
        for (i = 0; i < MAX_PLAYERS; ++i)
        {
            pthread_cancel(plthread[i]);
        }
        for (i = 0; i < MAX_TEAMS; ++i)
        {
            pthread_cancel(tthread[i]);
        }
        for (i = 0; i < MAX_PLAYERS; ++i)
        {
            pthread_join(plthread[i], NULL);
            pthread_cond_destroy(&(players[i].cond));
            pthread_mutex_destroy(&(players[i].mutex));
        }
        for (i = 0; i < MAX_TEAMS; ++i)
        {
            pthread_join(tthread[i], NULL);
            pthread_cond_destroy(&(teams[i].cond));
            pthread_mutex_destroy(&(teams[i].mutex));
        }
        pthread_mutex_destroy(&mutex[0]);
        free_mem();
       	return 0;
}

const char msgok[] = "hello";

void* playerthread(void* arg)
{
    struct sthread* ptr = (struct sthread*) arg;
    while (1)
    {
        pthread_cond_wait(&(ptr->cond), &(ptr->mutex)); 
        
        player_init();
        
        player_gameplay();
        
        close(ptr->desc);
        pthread_mutex_lock(&(ptr->mutex));
        pthread_mutex_lock(&(mutex[0]));

        --player_cnt;
        ptr->free = 1;
fdE
        pthread_mutex_unlock(&(mutex[0]));
        pthread_mutex_unlock(&(ptr->mutex));
    }
    return NULL;
}

void* teamthread(void* arg)
{
    struct sthread* ptr = (struct sthread*) arg;
    while (1)
    {
        pthread_cond_wait(&(ptr->cond), &(ptr->mutex)); 
        
        team_init();
        
        team_gameplay();
        
        close(ptr->desc);
        pthread_mutex_lock(&(ptr->mutex));
        pthread_mutex_lock(&(mutex[0]));

        --player_cnt;
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
    signal(SIGINT, handler);
   
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
                pthread_cond_signal(arr[i].cond);
                break;
            }
            pthread_mutex_unlock(&(arr[i].mutex));
        }
    } /* WHILE END */

    return 0;
}
