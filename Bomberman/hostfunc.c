#include "header.h"
/*
extern void err_report(const char* str, short flag);
extern void free_mem();*/

int host_handler(int sig){
    printf("SIGUSR2 catched\n");
    return 0;
}


void hostPlSend(struct sthread* ptr){
	msg_send(ptr->desc, MSG_HOST_PL, sizeof(int), &team_arr[ptr->team_id].pl_cnt);	
}
void hoStatSend(struct sthread* ptr){
	
}

void team_gameplay(struct sthread* ptr){
    int poll_ret;
    int flag = 1;
    struct pollfd polls;
    size_t size = 0;
    int type = 0;
    int msg_ret = 0;
    void* buf = NULL;
    
    printf("Team_game\n");
    polls.fd = ptr->desc;
    polls.events = POLLIN | POLLERR | POLLHUP;
    polls.revents = 0;
    while (flag)
    {
        printf("POLL\n");
        fflush(stdout);
        poll_ret = poll(&polls, 1, -1);
        
        if (poll_ret == -1)
        {
            hostPlSend(ptr);
        }
        if (polls.revents & POLLERR) {
            err_report("poll returned POLLERR\n", 0);
            return;
        }
        if (polls.revents & POLLHUP) {
            err_report("poll returned POLLHUP\n", 0);
            return;
        }
        if (polls.revents & POLLIN)
        {
          msg_ret = msg_rec(ptr->desc, &type, &size, &buf);
          if (type == MSG_INFO_STRING && !strcmp((char*)buf,start))
          {
            pthread_mutex_lock(&team_arr[ptr->team_id].mutex);
            team_arr[ptr->team_id].status = ST_PLAYING;
            flag = 0;
            pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);
          }
          if (type == MSG_INFO_STRING && !strcmp((char*)buf,disc))
          {
            free(buf);
            buf = NULL;
            disconnect(ptr);	
            return;
          }
				
        }
      
        polls.revents = 0;       
    }
	polls.revents = 0; 
  mapClone(&team_arr[ptr->team_id]);
	wakeSign(ptr->team_id);
	pause();
	wakeCond(ptr->team_id);
  msg_send(ptr->desc, MSG_INFO_STRING, strlen(start) + 1, start);
	msg_ret = clock_gettime(CLOCK_MONOTONIC, &team_arr[ptr->team_id].start);
    flag = 1;
    while (flag){
        poll_ret = poll(&polls, 1, -1);
        if (poll_ret == -1)
        {
            err_report("signal caught\n", 1);
        }
        if (polls.revents & POLLERR) {
            err_report("poll returned POLLERR\n", 0);
            return;
        }
        if (polls.revents & POLLHUP) {
            err_report("poll returned POLLHUP\n", 0);
            return;
        }
        
        if (polls.revents & POLLIN){
            msg_ret = msg_rec(ptr->desc, &type, &size, &buf);
            if (type == MSG_INFO_STRING || !strcmp((char*)buf, disc)){
                    flag = 0;
            }
    	}
      polls.revents = 0;   
    }
    free(buf);
    disconnect(ptr);
}
