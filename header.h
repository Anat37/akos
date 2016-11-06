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
void help_print()
{
	printf("help is not ready yet:)");
}
volatile int players_cnt;
volatile int teams_cnt;
volatile int team_arr_size = 0;
volatile int player_arr_size = 0;

volatile struct steam* team_arr;
volatile struct splayer* player_arr; 
volatile struct sthread* sthreads;
pthread_t* plthread;

pthread_mutex_t mutex[MUTEX_CNT];

unsigned port_num = 1050;
char* map_file_name = "defmap.map";
char* map_name;
char** map;
struct mobject* mobjects= NULL;
int mobjectscnt =  0;
int map_size_n;
int map_size_m;

float initial_health = 500;
float hit_value      = 50;
int recharge_duration = 3000;
int mining_time = 3000;
float stay_health_drop = 1;
float movement_health_drop = 4;
int step_standard_delay = 100;
int moratory_duration = 5000;



const char* set_send = "Sending settings";
const char* st_send = "Sending stat";
const char* conn_succ = "Connection accepted";
const char* conn_err = "Connection error. Server is full";
const char* disc = "Disconnect";
const char* start = "Starting game";
const char* wait_str = "Wait fo map";
const char* up_m = "Move up";
const char* dw_m = "Move down";
const char* lf_m = "Move left";
const char* rt_m = "Move right";
const char* put_mine = "Put mine";
const char* us_ob = "Use object";
const char* ch_st = "Change status";
const char* fire = "Use gun";
const char* succ_mov = "Move success";
const char* bad_mov = "Bad mov";

int err_solve(size_t size, void* buf){
	return 0;
};

void msg_send(int desc, int type, size_t len, void* arg)
{
	write(desc, (void*)&type, sizeof(int));
	write(desc, (void*)&len, sizeof(int));
	write(desc, arg, len);
	return; 
}

/*return -1 if succ, but eof; -2 fail, eof; -3 read error*/
int msg_rec(int desc, int* type, size_t* len, void** arg)
{
	int readed = 0;
	ssize_t read_ret;
	int ret = 0;
	char* str = NULL;
	free(*arg);
	while (readed < sizeof(int))
	{
		read_ret = read(desc, (void*)type + readed, sizeof(int) - readed);
		if (read_ret == -1)
			return -3;
		if (read_ret == 0)
		{
			err_report("Connection lost", 0);
			return -2;
		}
		readed += read_ret;
	}
	readed = 0;
	while (readed < sizeof(int))
	{
		read_ret = read(desc, (void*)len + readed, sizeof(int) - readed);
		if (read_ret == -1)
			return -3;
		if (read_ret == 0)
		{
			err_report("Connection lost", 0);
			return -2;
		}
		readed += read_ret;
	}
	str = malloc((unsigned int)*len);
	readed = 0;
	while (readed < *len)
	{
		read_ret = read(desc, str + readed, *len - readed);
		if (read_ret == -1)
			return -3;
		if (read_ret == 0)
		{
			err_report("Connection lost", 0);	
			break;
		}
		readed += read_ret;
	}
	*arg = str;
	if (*type == MSG_ERR)
	{
		ret = err_solve(*len, str);
		return (ret) ? ret : ((read_ret) ? 0 : -1);
	}
	return (read_ret) ? 0 : -1; 
}


long time_from(struct timespec from)
{
    struct timespec now;
    long ans = 0;
    int ret;
    ret = clock_gettime(CLOCK_MONOTONIC, &now);
    if (ret == -1 )
    {
        err_report("gettime error", 1);
        return -1;
    }
    ans = (now.tv_nsec - from.tv_nsec) / 1000000;
    ans += (now.tv_sec - from.tv_sec) * 1000;
    return ans;
}

struct timespec time_inc(struct timespec from, int k)
{
    struct timespec ans;
    ans.tv_sec = from.tv_sec + (k * step_standard_delay)/1000;
    ans.tv_nsec = from.tv_nsec + ((k * step_standard_delay)%1000) * 1000000;
    if (ans.tv_nsec/1000 > 1000000)
    {
        ++ans.tv_sec;
        ans.tv_nsec -= 1000000000;
    }
    return ans;
}

#endif
