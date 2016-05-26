

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
	ret = (int) *buf;
	team_arr[teams_cnt - 1].pl_cnt = 0;
	team_arr[teams_cnt - 1].ready_cnt = 0;
	team_arr[teams_cnt - 1].status = ST_WAITING;
	team_arr[teams_cnt - 1].max_pl_cnt = ret;
	team_arr[teams_cnt - 1].pl = malloc(ret * sizeof(int));
	ret = msg_rec(fd, &type, &size, &buf);
	if (ret != 0)
		return ret;
	team_arr[teams_cnt - 1].name = (char*) buf;
	buf = NULL;	
	return ret;
}

void host_handler(int sig){
    
}

void teamInit(struct sthread* ptr){
	
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
    size_t size;
    int type;
    int msg_ret = 0;
    void* buf = NULL;
    
    signal(SIGUSR2, &host_handler);
    polls.fd = ptr->desc;
    polls.events = POLLIN | POLLERR | POLLHUP;
    polls.revents = 0;
    while (flag){
        poll_ret = poll(polls, 1, -1);
        
        if (poll_ret == -1)
        {
            err_report("poll failed\n", 1);
            return;
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
            if (type == MSG_INFO_STRING && !strcmp((char*)buf,start)){
				pthread_mutex_lock(&team_arr[ptr->team_id].mutex);
				if (team_arr[ptr->team_id].status = ST_PLAYING)
				{
					flag = 0;
					pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);
					continue;
				}
				pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);
			}
			if (type == MSG_INFO_STRING && !strcmp((char*)buf,disc)){
				disconnect(ptr);	
				return;
			}
				
        }
		void hostPlSend(ptr);
        polls.revents = 0;       
    }
	polls.revents = 0; 
    mapClone(&team_arr[ptr->team_id]);
	teamInit(ptr);
	wakeSign(ptr->team_id);
	sleep();
	wakeCond(ptr->team_id);
    msg_send(ptr->desc, MSG_INFO_STRING, strlen(start) + 1, start);
	msg_ret = clock_gettime(CLOCK_MONOTONIC, &team_arr[ptr->team_id].start);
    flag = 1;
    while (flag){
        poll_ret = poll(polls, 1, -1);
        pthread_mutex_lock(&team_arr[ptr->team_id].mutex);
            if (team_arr[ptr->team_id].status = ST_ENDG)
            {
                flag = 0;
                msg_send(ptr->desc, MSG_INFO_STRING, strlen(disc) + 1, disc);
                pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);
                continue;
            }
        pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);
        if (poll_ret == -1)
        {
            err_report("poll failed\n", 1);
            return;
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
    
        polls.revents = 0;   
    }
    disconnect(ptr);
}