#include"defs.h"


unsigned port_num = DEF_PORT;
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
int mode = 0;
	

char** map = NULL;


void err_report(const char* str, short flag)	/*flag for system call error*/
{
	if (flag)
		perror(str);
	else printf("%s\n", str);	
}


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
                  exit(0);
    				break;
        	}	
        }	
}


void init_me()
{
	int i;
	int msg_ret;
	int type = 0;
	size_t size = 0;
	void* buf = NULL;
	int teams_cnt = 0;
	int status = -1;
  int flag = 1;
  printf("Initing\n");
	msg_ret = msg_rec(sock_id, &type, &size, &buf);
	if (type == MSG_INFO_STRING && !strcmp((char*) buf ,conn_err)){
		printf("Error connecting to server");
		close(sock_id);
		exit(0);
	}
	
  printf("%s\n",buf);
	
	
	msg_ret = msg_rec(sock_id, &type, &size, &buf);
	teams_cnt = *((int*) buf);
  
  free(buf);
  buf = NULL;
	printf("There are %d rooms on server now:\n", teams_cnt);
	for (i = 0; i < teams_cnt; ++i)
	{
		msg_ret = msg_rec(sock_id, &type, &size, &buf);
		printf("Room%d: %s",i, (char*)buf);
		msg_ret = msg_rec(sock_id, &type, &size, &buf);
		status =*((int*) buf);
    free(buf);
    buf = NULL;
		printf("Status:");
		switch(status){
			case ST_READY: printf("FULL\n"); break;
			case ST_PLAYING:printf("PLAYING\n"); break;
			case ST_WAITING:printf("WAITING\n"); break;
      case ST_ENDG: printf("ENDED\n"); break;
		}
	}
	flag = teams_cnt;
	while (flag){
		printf("If you want to create new room type in -1\nIf you want to join room type in its number\n");
		scanf("%d", &status);
		if (status >= teams_cnt || status < -1)
		{
			printf("Invalid value, try again\n");
		}
			else flag = 0;
	}
	msg_send(sock_id, MSG_INFO_NUM, sizeof(int), (void*)&status);
  buf = malloc(80*sizeof(char));
	if (status == -1)
	{
		cltype = 2;
    printf("Enter smth\n");
    fgets(buf, 60, stdin);
		printf("Please enter your room name(no more than 60 symbols):");
		fgets(buf, 60, stdin);
    printf("Please enter maximum number of players:");
    scanf("%d", &teams_cnt);
    
		msg_send(sock_id, MSG_INFO_NUM, sizeof(int), (void*)&teams_cnt);
	}
	else 
	{
		cltype = 1;
    printf("Enter smth\n");
    fgets(buf, 60, stdin);
		printf("Please enter your player name(no more than 60 symbols):");
    fgets(buf, 60, stdin);
	}
  fflush(stdin);
	msg_send(sock_id, MSG_INFO_STRING, strlen((char*)buf) + 1, (void*)buf);
	map = malloc(21*sizeof(char*));
  printf("End INIt\n");
	free(buf);
	return;
}

void mapRec()
{
    int i;
    int readed = 0;
	int ret;
	int type = 0;
	size_t size = 0;
    for (i = 0; i < 20; ++i)
    {
		ret = msg_rec(sock_id, &type, &size, (void**)&map[i]);
    } 
    reprint();
}
void setRec(){
	int type = 0;
	size_t size = 0;
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
  
  free(buf);
}

void statRec(){
	int type = 0;
	size_t size = 0;
	void* buf = NULL;
    msg_rec(sock_id, &type, &size, (void**)&buf);
	x = *((int*)buf);
    msg_rec(sock_id, &type, &size, (void**)&buf);
	y = *((int*)buf);
    msg_rec(sock_id, &type, &size, (void**)&buf);
    hp = *((float*)buf);
    fflush(stdout);
	msg_rec(sock_id, &type, &size, (void**)&buf);
    minecnt = *((int*)buf);
	msg_rec(sock_id, &type, &size, (void**)&buf);
    minecd = *((int*)buf);
	msg_rec(sock_id, &type, &size, (void**)&buf);
    guncd = *((int*)buf);
  free(buf);
}

void init_game()
{
  int i =0;
  int type = 0;
	size_t size = 0;
	void* buf = NULL;
  msg_rec(sock_id, &type, &size, (void**)&buf);
	setRec();
  for (i = 0; i < 20; ++i)
    map[i] = NULL;
  msg_rec(sock_id, &type, &size, (void**)&buf);
	mapRec();
  msg_rec(sock_id, &type, &size, (void**)&buf);
	statRec();
  free(buf);
}
void handler(int sig)
{
  int i;
  if (mode) to_canonical();
  msg_send(sock_id, MSG_INFO_STRING, strlen(disc) + 1, (void*)disc);
  
  if (cltype == 1)
    for (i = 0; i < 20; ++i)
      free(map[i]);
  free(map);
  sleep(1);
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
    size_t size = 0;
    int type = 0;
	uint64_t num = 0;
    int msg_ret = 0;
    void* buf = NULL;
	struct itimerspec isp;
  polls[0].fd = STDIN_FILENO;
	polls[0].events = POLLIN | POLLERR | POLLHUP;
	polls[0].revents = 0;
	polls[1].fd = sock_id;
	polls[1].events = POLLIN | POLLERR | POLLHUP;
	polls[1].revents = 0;
	polls[2].events = POLLIN | POLLERR | POLLHUP;
	polls[2].revents = 0;
  
	init_game();
  isp.it_interval.tv_sec = step_standard_delay/1000;  
	isp.it_interval.tv_nsec =(step_standard_delay%1000) * 1000000;
  isp.it_value.tv_sec = 1; 
	isp.it_value.tv_nsec = 1;
  
  polls[2].fd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (polls[2].fd < 0)
    err_report("timer settime fail\n", 1);
    
	msg_ret = msg_rec(sock_id, &type, &size, &buf);
    if (strcmp((char*)buf, start))
    {
      printf("watch it\n");
      pause();
    }  
  msg_ret = clock_gettime(CLOCK_MONOTONIC, &lastmove);
  msg_ret = clock_gettime(CLOCK_MONOTONIC, &frstart);
	msg_ret = timerfd_settime(polls[2].fd, 0, &isp, NULL);
  if (msg_ret == -1)
    err_report("timer settime fail\n", 1);
  reprint();
  while (flag)
  {
    poll_ret = poll(polls, 3, -1);
    if (poll_ret == -1)
    {
      err_report("poll failed\n", 1);
      handler(SIGINT);
    }
      
    if (polls[0].revents & POLLERR || polls[1].revents & POLLERR || polls[2].revents & POLLERR) 
    {
      err_report("poll returned POLLERR\n", 0);
      handler(SIGINT);
    }
      
    if (polls[0].revents & POLLHUP || polls[1].revents & POLLHUP || polls[2].revents & POLLHUP) 
    {
      err_report("poll returned POLLHUP\n", 0);
      handler(SIGINT);
    }

    if (polls[2].revents & POLLIN)
    {
      read(polls[2].fd, (void*)&num, sizeof(uint64_t));
      printf("Timer work");
      dec_health(num);
      polls[2].revents = 0;
    }
     
    if (polls[1].revents & POLLIN)
    {
      polls[1].revents = 0;
      msg_ret = msg_rec(sock_id, &type, &size, &buf);
      if (type == MSG_INFO_STRING)
      {
        rec_pkg(buf);
        
      }
			free(buf);
      buf = NULL;
      if (status == ST_DEAD)
      {
        printf("Game Over");
        flag = 0;
        handler(SIGINT);
      }
    }
      
    if (polls[0].revents & POLLIN)
    {
      user_input(cltype);
      polls[0].revents = 0; 
    }
    
    if (status == ST_DEAD)
    {
      printf("Game Over");
      flag = 0;
      handler(SIGINT);
    }
       
	}
  free(buf);
}
void player_wait(){
	int poll_ret;
    int flag = 1;
    struct pollfd polls[3];
    size_t size = 0;
    int type = 0;
    int msg_ret = 0;
    void* buf = NULL;
    polls[0].fd = STDIN_FILENO;
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
				flag = 0;
        printf("Starting\n");
        fflush(stdout);
				continue;
			}
      free(buf);
      buf = NULL;
        }
		if (polls[0].revents & POLLIN){
			user_input(cltype);
		}
		polls[1].revents = 0;
        polls[0].revents = 0; 
	}
  free(buf);
	player_game();
}


void host_wait(){
	int poll_ret;
    int flag = 1;
    struct pollfd polls[2];
    size_t size = 0;
    int type = 0;
    int msg_ret = 0;
    void* buf = NULL;
    polls[0].fd = STDIN_FILENO;
	polls[0].events = POLLIN | POLLERR | POLLHUP;
	polls[0].revents = 0;
	polls[1].fd = sock_id;
	polls[1].events = POLLIN | POLLERR | POLLHUP;
	polls[1].revents = 0;
    while (flag)
    {
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
              printf("Number of players: %d\n", *((int*)buf));
              free(buf);
              buf = NULL;
            }
        }
        if (polls[0].revents & POLLIN)
        {
          user_input(cltype);
          if (game_status == 1){
            msg_send(sock_id, MSG_INFO_STRING, strlen(start) + 1, (void*)start);	
            game_status++;
          }
        }
        
        polls[1].revents = 0;
        polls[0].revents = 0; 
	}
	
	
}
int main(int argc, char* argv[])
{
	options(argc, argv);
	
	sock_id = socket(PF_INET, SOCK_STREAM, 0);
    hst = gethostbyname(hostname);
   
   	server_addr.sin_family = AF_INET;
   	server_addr.sin_port = htons((short)port_num);
   	memcpy(&(server_addr.sin_addr.s_addr), hst->h_addr_list[0], hst->h_length);
   
  if (connect(sock_id, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr_in)) == -1)
  {
   	err_report("connect fail\n", 0);
   	return -2;
  }
	signal(SIGINT, &handler);
	init_me();
	to_noncanonical();
  mode = 1;
	if (cltype == 1)
		player_wait();
	else
		host_wait();
   
  printf("\nEND\n");
	to_canonical();
  mode = 0;
  close(sock_id);
	return 0;
}

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4 

void move(int k)
{
	int type = 0;
	size_t size = 0;
	void* buf = NULL;
	int ret;
	if (time_from(lastmove) < 1 || minecd > recharge_duration)
    {
        printf("To fast\n");
        return;
    }
	switch (k){
		case UP:if (map[9][10] == ' ' || map[9][10] == '+' || map[9][10] == 'F'){
					msg_send(sock_id, MSG_INFO_STRING, strlen(up_m) + 1, (void*)up_m);
					ret = msg_rec(sock_id, &type, &size, &buf);}
				break;
		case DOWN: if (map[11][10] == ' ' || map[11][10] == '+' || map[11][10] == 'F'){
					msg_send(sock_id, MSG_INFO_STRING, strlen(dw_m) + 1, (void*)dw_m);
					ret = msg_rec(sock_id, &type, &size, &buf);}
					break;
		case LEFT:if (map[10][9] == ' ' || map[10][9] == '+' || map[10][9] == 'F'){
					msg_send(sock_id, MSG_INFO_STRING, strlen(lf_m) + 1, (void*)lf_m);
					ret = msg_rec(sock_id, &type, &size, &buf);} 
					break;
		case RIGHT: if (map[10][11] == ' ' || map[10][11] == '+' || map[10][11] == 'F'){
					msg_send(sock_id, MSG_INFO_STRING, strlen(rt_m) + 1, (void*)rt_m);
					ret = msg_rec(sock_id, &type, &size, &buf);}
					break;
    default: printf("smth wrong!\n"); break;
	}	
  free(buf);
}

void mine(){
	int type = 0;
	size_t size = 0;
	void* buf = NULL;
	int ret;
	if (time_from(lastmove) < 1 || time_from(frstart) < moratory_duration/step_standard_delay || minecd > 0)
    {
      printf("To fast\n");
      return;
    }
	msg_send(sock_id, MSG_INFO_STRING, strlen(put_mine) + 1, (void*)put_mine);
	ret = msg_rec(sock_id, &type, &size, &buf);
  free(buf);
} 
void use_obj()
{
	int type = 0;
	size_t size = 0;
	void* buf = NULL;
	int ret;
	if (time_from(lastmove) < 1 || minecd > recharge_duration)
    {
        printf("To fast\n");
        return;
    }
	msg_send(sock_id, MSG_INFO_STRING, strlen(us_ob) + 1, (void*)us_ob);
	ret = msg_rec(sock_id, &type, &size, &buf);
  free(buf);
} 	
void fireuse(){
	int type = 0;
	size_t size = 0;
	void* buf = NULL;
	int ret;
	if (time_from(lastmove) < 1 || time_from(frstart) < moratory_duration/step_standard_delay || guncd > 0 || minecd > recharge_duration)
    {
        printf("To fast\n");
        return;
    }
	msg_send(sock_id, MSG_INFO_STRING, strlen(fire) + 1, (void*)fire);
	ret = msg_rec(sock_id, &type, &size, &buf);
  free(buf);
} 
