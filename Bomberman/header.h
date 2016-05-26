#define _POSIX_C_SOURCE 199309L

#include <pwd.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>
#include <poll.h>
#include <math.h>
#include <time.h>
#include <sys/timerfd.h>

#define OBJ_MAP 501
#define OBJ_PL 502
#define OBJ_MINE 503
#define ST_WAITING 201
#define ST_READY 202
#define ST_PLAYING 203
#define ST_DEAD 204
#define ST_ALIVE 205
#define ST_ENDG 206

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
void help_print()
{
	printf("help is not ready yet:)");
}

unsigned daemon_flag = 0;
float initial_health = 500;
float hit_value      = 50;
int recharge_duration = 3000;
int mining_time = 3000;
float stay_health_drop = 1;
float movement_health_drop = 4;
int step_standard_delay = 100;
int moratory_duration = 5000;

void err_report(const char* str, short flag);
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