#define _POSIX_C_SOURCE 200112L
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>


#define MSG_INFO_STRING 1
#define MSG_INFO_NUM 2
#define MSG_ERR 3
#define MSG_MAP_W 4
#define MSG_MAP_STRING 5
#define MSG_MAP_NUM 6
#define MSG_STAT_NUM 7
#define MSG_SET_NUM 8
#define MSG_HOST_PL 9

extern int step_standard_delay;

int err_solve(size_t size, void* buf){
	return 0;
};


void msg_send(int desc, int type, size_t len, void* arg)
{
  int ret;
  ret = write(desc, (void*)&type, sizeof(int));
  if (ret == sizeof(int))
    /*printf("Sended type:%d\n", type);*/
	ret = write(desc, (void*)&len, sizeof(int));
  if (ret == sizeof(int))
    /*printf("Sended size:%d\n", len);*/
	ret = write(desc, arg, len);
  /*if (type == 1 || type == 4 || type == 5)
    printf("Sended buf:%s\n", (char*)arg);
  if (ret != len)
    printf("problems\n");*/
	return; 
}

int msg_rec(int desc, int* type, size_t* len, void** arg)
{
	int readed = 0;
	ssize_t read_ret;
	int ret = 0;
	void* str = NULL;
  struct pollfd msgpoll;
  msgpoll.fd = desc;
  msgpoll.events = POLLIN | POLLERR;
  msgpoll.revents = 0;
	free(*arg);
	while (readed < sizeof(int))
	{
    
    poll(&msgpoll, 1, -1);
    msgpoll.revents = 0;
		read_ret = read(desc, (void*)type + readed, sizeof(int) - readed);
		if (read_ret == -1)
			return -3;
		if (read_ret == 0)
		{
			err_report("Connection lost", 0);
      close(desc);
      raise(SIGPIPE);
			return -2;
		}
		readed += read_ret;
	}
  /*printf("Received type %d\n", *type);*/
	readed = 0;
	while (readed < sizeof(int))
	{ 
    poll(&msgpoll, 1, -1);
    msgpoll.revents = 0;
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
  /*printf("Received len %d\n", *len);*/
	str = malloc((unsigned int)*len);
	readed = 0;
	while (readed < *len)
	{ 
    poll(&msgpoll, 1, -1);
    msgpoll.revents = 0;
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
  /*
  if (*type == 1 || *type == 4 || *type == 5)
    printf("Received buf %s\n", (char*)str);
  else
    printf("Received buf %d\n", *(int*)str);
  fflush(stdout);
  */
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
    printf("step _ delay%d\n", &step_standard_delay);
    if (ans.tv_nsec/1000 > 1000000)
    {
        ++ans.tv_sec;
        ans.tv_nsec -= 1000000000;
    }
    return ans;
}

