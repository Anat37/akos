#define _POSIX_C_SOURCE 200112L
#ifndef _SECONDHEAD
#define _SECONDHEAD



#include <pwd.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>
#include <poll.h>
#include <time.h>
#include <sys/timerfd.h>
#include <unistd.h>

#define DEF_PORT 12005


#define OBJ_MAP 501
#define OBJ_PL 502
#define OBJ_MINE 503
#define ST_WAITING 201
#define ST_READY 202
#define ST_PLAYING 203
#define ST_DEAD 204
#define ST_ALIVE 205
#define ST_ENDG 206


#define MAGIC_CONST 5
#define THREADS_INIT 16
#define T_PLAYER 101
#define T_HOST 102
#define MUTEX_CNT 5


#define MSG_INFO_STRING 1
#define MSG_INFO_NUM 2
#define MSG_ERR 3
#define MSG_MAP_W 4
#define MSG_MAP_STRING 5
#define MSG_MAP_NUM 6
#define MSG_STAT_NUM 7
#define MSG_SET_NUM 8
#define MSG_HOST_PL 9

static const char* set_send = "Sending settings";
static const char* st_send = "Sending stat";
static const char* conn_succ = "Connection accepted";
static const char* conn_err = "Connection error. Server is full";
static const char* disc = "Disconnect";
static const char* start = "Starting game";
static const char* wait_str = "Wait fo map";
static const char* up_m = "Move up";
static const char* dw_m = "Move down";
static const char* lf_m = "Move left";
static const char* rt_m = "Move right";
static const char* put_mine = "Put mine";
static const char* us_ob = "Use object";
static const char* ch_st = "Change status";
static const char* fire = "Use gun";
static const char* succ_mov = "Move success";
static const char* bad_mov = "Bad mov";

float initial_health;
float hit_value;
int recharge_duration;
int mining_time;
float stay_health_drop;
float movement_health_drop;
int step_standard_delay;
int moratory_duration;

static void help_print()
{
	printf("help is not ready yet:)");
}

#endif
