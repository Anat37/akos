#include "header.h"


unsigned port_num = 1050;
char* hostname = "localhost"; 
int sock_id;
int cltype;
struct hostent *hst;
struct sockaddr_in server_addr;
FILE* file;
int game_status = 0;
volatile int status;
volatile float hp;
volatile int minecnt = 6;
volatile int minecd;
volatile int guncd;
volatile struct timespec lastmove;
volatile struct timespec frstart; 
volatile int x;
volatile int y;
	

char** map = NULL;


void err_report(const char* str, short flag)	/*flag for system call error*/
{
	if (flag)
		perror(str);
		else printf("%s\n", str);	
}
#include "msg.h"
void options(int argc, char* argv[])
{
	char *opts = "s:p:h";/* hostname, port, help*/
    	int opt; 
    	while((opt = getopt(argc, argv, opts)) != -1)
    	{  
    		switch ((char)opt)
    		{
    			case 'p': port_num = atoi(optarg);
    				if (port_num > 10000)
    					{
    						printf("Invalod port number");
    						port_num = 1050;	
    					}
    				break;
    			case 's':hostname = optarg;
    				break; 
    			case 'h': help_print();
    				break;
        	}	
        }	
}


void init_me()
{
	int i;
	int msg_ret;
	int type;
	size_t size;
	void* buf = NULL;
	int teams_cnt = 0;
	int status;
	short flag = 1;
	FILE* fstr;
	msg_ret = msg_rec(sock_id, &type, &size, &buf);
	if (type == MSG_INFO_STRING && !strcmp((char*) buf ,conn_err)){
		printf("Error connecting to server");
		close(sock_id);
		exit(0);
	}
	
	
	
	msg_ret = msg_rec(sock_id, &type, &size, &buf);
	teams_cnt = *((int*) buf);
	printf("There are %d rooms on server now:\n", teams_cnt);
	for (i = 0; i < teams_cnt; ++i)
	{
		msg_ret = msg_rec(sock_id, &type, &size, &buf);
		printf("Room%d: %s",i, (char*)buf);
		msg_ret = msg_rec(sock_id, &type, &size, &buf);
		status =*((int*) buf);
		printf("Status:");
		switch(status){
			case ST_READY: printf("FULL\n"); break;
			case ST_PLAYING:printf("PLAYING\n"); break;
			case ST_WAITING:printf("WAITING\n"); break;
		}
	}
	flag = teams_cnt;
	while (flag){
		printf("If you want to create new room type in -1\nIf you want to join room type in its number\n");
		scanf("%d", &status);
		if (status >= teams_cnt || status < -1)
		{
			printf("Invalid value, try again");
		}
			else flag = 0;
	}
	if (teams_cnt)
		status = -1;
	msg_send(sock_id, MSG_INFO_NUM, sizeof(int), (void*)&status);
	fstr = fdopen(0, "r");
	flag = 1;
	if (status == -1)
	{
		cltype = 2;
		while(flag)
		{
			printf("Please enter your room name(no more than 60 symbols):");
			safe_gets(fstr, &buf);
			if (strlen(buf) > 60)
				printf("Please enter shorter name\n");
			else flag = 0;
		}
		flag = 0;
		while(flag)
		{
			printf("Please enter maximum number of players:");
			scanf("%d", &teams_cnt);
			if (teams_cnt < 0)
				printf("Please enter valid number\n");
			else flag = 0;
		}
		msg_send(sock_id, MSG_INFO_NUM, sizeof(int), (void*)&teams_cnt);
	}
	else 
	{
		cltype = 1;
		while(flag)
		{
			printf("Please enter your player name(no more than 60 symbols):");
			safe_gets(fstr, &buf);
			if (strlen(buf) > 60)
				printf("Please enter shorter name\n");
			else flag = 0;
		}
	}
	msg_send(sock_id, MSG_INFO_STRING, strlen(buf) + 1, (void*)buf);
	map = malloc(21*sizeof(char*));
	fclose(fstr);
	free(buf);
	return;
}

void mapRec()
{
    int i;
    int readed = 0;
	int ret;
	int type;
	size_t size;
    for (i = 0; i < 20; ++i)
    {
		ret = msg_rec(sock_id, &type, &size, (void**)&map[i]);
    }    
}
void setRec(){
	int type;
	size_t size;
	void* buf = NULL;
    msg_rec(sock_id, &type, &size, (void**)&buf);
	hit_value = *((float*)buf);
    msg_rec(sock_id, &type, &size, (void**)&buf);
	recharge_duration = *((int*)buf);
    msg_rec(sock_id, &type, &size, (void**)&buf);
    mining_time = *((int*)buf);
	msg_rec(sock_id, &type, &size, (void**)&buf);
    stay_health_drop = *((float*)buf);
	msg_rec(sock_id, &type, &size, (void**)&buf);
    movement_health_drop = *((float*)buf);
	msg_rec(sock_id, &type, &size, (void**)&buf);
    step_standard_delay = *((int*)buf);
	msg_rec(sock_id, &type, &size, (void**)&buf);
	moratory_duration = *((int*)buf);
}

void statRec(){
	int type;
	size_t size;
	void* buf = NULL;
    msg_rec(sock_id, &type, &size, (void**)&buf);
	x = *((int*)buf);
    msg_rec(sock_id, &type, &size, (void**)&buf);
	y = *((int*)buf);
    msg_rec(sock_id, &type, &size, (void**)&buf);
    hp = *((float*)buf);
	msg_rec(sock_id, &type, &size, (void**)&buf);
    minecnt = *((int*)buf);
	msg_rec(sock_id, &type, &size, (void**)&buf);
    minecd = *((int*)buf);
	msg_rec(sock_id, &type, &size, (void**)&buf);
    guncd = *((int*)buf);
}

void init_game(){
	setRec();
	mapRec();
	statRec();
}
void handler(int sig){
	close(sock_id);
	exit(0);
}
void dec_health(uint64_t k){
    minecd -= k * step_standard_delay;
    guncd -= k * step_standard_delay;
    hp -= stay_health_drop * k;
    if (hp < 0)
    {
        status = ST_DEAD;
        msg_send(sock_id, MSG_INFO_STRING, strlen(ch_st) + 1, (void*)ch_st);
        return;
    }
	reprint();
}

void rec_pkg(char* str){
	if (!strcmp(str, set_send))
    {
        setRec();
        return ;
    }
	if (!strcmp(str, wait_str))
    {
        mapRec();
        return ;
    }
	if (!strcmp(str, st_send))
    {
        statRec();
        return ;
    }
	if (!strcmp(str, disc))
    {
		free(str);
        handler(SIGINT);
        return ;
    }
	
}
void player_game(){
	int poll_ret;
    int flag = 1;
    struct pollfd polls[3];
    size_t size;
    int type;
	uint64_t num;
    int msg_ret = 0;
    void* buf = NULL;
	struct itimerspec isp;
    polls[0].fd = 0;
	polls[0].events = POLLIN | POLLERR | POLLHUP;
	polls[0].revents = 0;
	polls[1].fd = sock_id;
	polls[1].events = POLLIN | POLLERR | POLLHUP;
	polls[1].revents = 0;
	polls[2].fd = timerfd_create(CLOCK_MONOTONIC, O_NONBLOCK);
	polls[2].events = POLLIN | POLLERR | POLLHUP;
	polls[2].revents = 0;
	isp.it_interval.tv_sec = step_standard_delay/1000;  
	isp.it_interval.tv_sec = (step_standard_delay%1000) * 1000000;
    isp.it_value.tv_sec = 0; 
	isp.it_value.tv_sec = 1;
	msg_ret = msg_rec(sock_id, &type, &size, &buf);
	msg_ret = clock_gettime(CLOCK_MONOTONIC, &lastmove);
    msg_ret = clock_gettime(CLOCK_MONOTONIC, &frstart);
	msg_ret = timerfd_settime(polls[2].fd, 0, &isp, NULL);
    while (flag){
        poll_ret = poll(polls, 3, -1);
		if (poll_ret == -1)
        {
            err_report("poll failed\n", 1);
            handler(SIGINT);
        }
        if (polls[0].revents & POLLERR || polls[1].revents & POLLERR || polls[2].revents & POLLERR) {
            err_report("poll returned POLLERR\n", 0);
            handler(SIGINT);
        }
        if (polls[0].revents & POLLHUP || polls[1].revents & POLLHUP || polls[2].revents & POLLERR) {
            err_report("poll returned POLLHUP\n", 0);
            handler(SIGINT);
        }
		if (polls[2].revents & POLLIN){
			read(polls[2].fd, sizeof(uint64_t), (void*)&num);
			dec_health(num);
			polls[2].revents = 0;
		}
        if (polls[1].revents & POLLIN){
            msg_ret = msg_rec(sock_id, &type, &size, &buf);
            if (type == MSG_INFO_STRING){
				rec_pkg(buf);
			}
			reprint();
			polls[1].revents = 0;
        }
		if (polls[0].revents & POLLIN){
			user_input(cltype);
			reprint();
			polls[0].revents = 0; 
		}
		if (status == ST_DEAD)
		{
			printf("Game Over");
			flag = 0;
			handler(SIGINT);
		}
        
	}
}
void player_wait(){
	int poll_ret;
    int flag = 1;
    struct pollfd polls[3];
    size_t size;
    int type;
    int msg_ret = 0;
    void* buf = NULL;
    polls[0].fd = 0;
	polls[0].events = POLLIN | POLLERR | POLLHUP;
	polls[0].revents = 0;
	polls[1].fd = sock_id;
	polls[1].events = POLLIN | POLLERR | POLLHUP;
	polls[1].revents = 0;
	printf("Waiting for gamehost to start");
    while (flag){
        poll_ret = poll(polls, 2, -1);
		if (poll_ret == -1)
        {
            err_report("poll failed\n", 1);
            handler(SIGINT);
        }
        if (polls[0].revents & POLLERR || polls[1].revents & POLLERR) {
            err_report("poll returned POLLERR\n", 0);
            handler(SIGINT);
        }
        if (polls[0].revents & POLLHUP || polls[1].revents & POLLHUP) {
            err_report("poll returned POLLHUP\n", 0);
            handler(SIGINT);
        }
        if (polls[1].revents & POLLIN){
            msg_ret = msg_rec(sock_id, &type, &size, &buf);
            if (type == MSG_INFO_STRING && !strcmp((char*)buf,start)){
				init_game();
				flag = 0;
				continue;
			}
        }
		if (polls[0].revents & POLLIN){
			user_input(cltype);
		}
		polls[1].revents = 0;
        polls[0].revents = 0; 
	}
	player_game();
}


void host_wait(){
	int poll_ret;
    int flag = 1;
    struct pollfd polls[2];
    size_t size;
    int type;
    int msg_ret = 0;
    void* buf = NULL;
    polls[0].fd = 0;
	polls[0].events = POLLIN | POLLERR | POLLHUP;
	polls[0].revents = 0;
	polls[1].fd = sock_id;
	polls[1].events = POLLIN | POLLERR | POLLHUP;
	polls[1].revents = 0;
    while (flag){
        poll_ret = poll(polls, 2, -1);
		if (poll_ret == -1)
        {
            err_report("poll failed\n", 1);
            handler(SIGINT);
        }
        if (polls[0].revents & POLLERR || polls[1].revents & POLLERR) {
            err_report("poll returned POLLERR\n", 0);
            handler(SIGINT);
        }
        if (polls[0].revents & POLLHUP || polls[1].revents & POLLHUP) {
            err_report("poll returned POLLHUP\n", 0);
            handler(SIGINT);
        }
        if (polls[1].revents & POLLIN){
            msg_ret = msg_rec(sock_id, &type, &size, &buf);
            if (type == MSG_HOST_PL){
				printf("Number of players: %d", *((int*)buf));
            }
        }
		if (polls[0].revents & POLLIN){
			user_input(cltype);
			if (game_status == 1){
				msg_send(sock_id, MSG_INFO_STRING, strlen(start) + 1, (void*)start);	
			}
		}
		polls[1].revents = 0;
        polls[0].revents = 0; 
	}
	
	
}
int main(int argc, char* argv[])
{
	int i = 6;
	int type = 0;
	size_t len = 0;
	char* buf = NULL;
	options(argc, argv);
	
	sock_id = socket(PF_INET, SOCK_STREAM, 0);
    hst = gethostbyname(hostname);
   
   	server_addr.sin_family = AF_INET;
   	server_addr.sin_port = htons((short)port_num);
   	memcpy(&(server_addr.sin_addr.s_addr), hst->h_addr_list[0], hst->h_length);
   
   	if (connect(sock_id, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr_in)) == -1)
   	{
       	printf("connect fail\n");
       	return -2;
   	}
	signal(SIGINT, &handler);
	printf("ok\n");
	init_me();
	to_noncanonical();
	if (cltype == 1)
		player_wait();
	else
		host_wait();
   
    printf("\nEND\n");
	to_canonical();
    free(buf);
    close(sock_id);
	return 0;
}

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4 

void move(int k)
{
	int type;
	size_t size;
	void* buf;
	int ret;
	if (time_from(lastmove) < step_standard_delay || minecd > recharge_duration)
    {
        return;
    }
	switch (k){
		case UP:if (map[9][10] == '*' || map[9][10] == '+'){
					msg_send(sock_id, MSG_INFO_STRING, strlen(up_m) + 1, (void*)up_m);
					ret = msg_rec(sock_id, &type, &size, &buf);}
				break;
		case DOWN: if (map[11][10] == '*' || map[11][10] == '+'){
					msg_send(sock_id, MSG_INFO_STRING, strlen(dw_m) + 1, (void*)dw_m);
					ret = msg_rec(sock_id, &type, &size, &buf);}
					break;
		case LEFT:if (map[10][9] == '*' || map[10][9] == '+'){
					msg_send(sock_id, MSG_INFO_STRING, strlen(lf_m) + 1, (void*)lf_m);
					ret = msg_rec(sock_id, &type, &size, &buf);} 
					break;
		case RIGHT: if (map[10][11] == '*' || map[10][11] == '+'){
					msg_send(sock_id, MSG_INFO_STRING, strlen(rt_m) + 1, (void*)rt_m);
					ret = msg_rec(sock_id, &type, &size, &buf);}
					break;
	}	
}

void mine(){
	int type;
	size_t size;
	void* buf;
	int ret;
	if (time_from(lastmove) < step_standard_delay || time_from(frstart) < moratory_duration || minecd > 0)
    {
        return;
    }
	msg_send(sock_id, MSG_INFO_STRING, strlen(put_mine) + 1, (void*)put_mine);
	ret = msg_rec(sock_id, &type, &size, &buf);
} 
void use_obj()
{
	int type;
	size_t size;
	void* buf;
	int ret;
	if (time_from(lastmove) < step_standard_delay || minecd > recharge_duration)
    {
        return;
    }
	msg_send(sock_id, MSG_INFO_STRING, strlen(us_ob) + 1, (void*)us_ob);
	ret = msg_rec(sock_id, &type, &size, &buf);
} 	
void fireuse(){
	int type;
	size_t size;
	void* buf;
	int ret;
	if (time_from(lastmove) < step_standard_delay || time_from(frstart) < moratory_duration || guncd > 0 || minecd > recharge_duration)
    {
        return;
    }
	msg_send(sock_id, MSG_INFO_STRING, strlen(fire) + 1, (void*)fire);
	ret = msg_rec(sock_id, &type, &size, &buf);
} 