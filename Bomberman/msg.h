#define MSG_INFO_STRING 1
#define MSG_INFO_NUM 2
#define MSG_ERR 3
#define MSG_MAP_W 4
#define MSG_MAP_STRING 5
#define MSG_MAP_NUM 6
#define MSG_STAT_NUM 7
#define MSG_SET_NUM 8
#define MSG_HOST_PL 9

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
