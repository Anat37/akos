#ifndef _FIRSTHEAD
#define _FIRSTHEAD

#include"defs.h"
struct mobject
{
	int x;
	int y;
	int type;
	int arg;
};

struct object
{
	int type;
	int arg;
	int owner;
};
struct splayer{
	int thread_id;
	int x;
	int y;
	int status;
	float hp;
	int minecnt;
	int minecd;
	int guncd;
	struct timespec lastmove;
	struct timespec lasthp;
	char* name;
};

struct sthread {
    int free;
    int desc;
    int type; 
	int id;
	int team_id;
	int pl;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

struct steam {
	int thread_id;
	int max_pl_cnt;
	int pl_cnt;
	int ready_cnt;
	int status;
	struct timespec start;
	int* pl;
	char* name;
	char** map;
	struct object** objects;
	pthread_mutex_t* map_mutex;
	pthread_mutex_t mutex;
};






volatile struct steam* team_arr;
volatile struct splayer* player_arr; 
volatile struct sthread* sthreads;
pthread_t* plthread;

pthread_mutex_t mutex[MUTEX_CNT];

unsigned port_num;
char* map_file_name;
char* map_name;
char** map;
struct mobject* mobjects;
int mobjectscnt;
int map_size_n;
int map_size_m;







int err_solve(size_t size, void* buf);

void msg_send(int desc, int type, size_t len, void* arg);

/*return -1 if succ, but eof; -2 fail, eof; -3 read error*/
int msg_rec(int desc, int* type, size_t* len, void** arg);


long time_from(struct timespec from);

struct timespec time_inc(struct timespec from, int k);

#endif
