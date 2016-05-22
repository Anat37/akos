#define MSG_INFO_STRING 1


char* conn_succ = "Connection accepted";
char* conn_err = "Connection error. Server is full";
int err_solve(size_t size, void* buf);

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
	char* str = NULL;
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
		ret = err_solve(*len, buf);
		return (ret) ? ret : ((read_ret) ? 0 : -1);
	}
	return (read_ret) ? 0 : -1; 
}
