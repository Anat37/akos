#define MSG_INFO_STRING 1


char* conn_succ = "Connection accepted";
char* conn_err = "Connection error. Server is full";

void msg_send(int desc, int type, size_t len, void* arg)
{
	write(desc, (void*)&type, sizeof(int));
	write(desc, (void*)&len, sizeof(int));
	write(desc, arg, len);
	return; 
}

int msg_rec(int desc, int* type, size_t* len, void** arg)
{
	int readed = 0;
	ssize_t read_ret;
	char* str = NULL;
	while (readed < sizeof(int))
	{
		read_ret = read(desc, (void*)type + readed, sizeof(int) - readed);
		if (read_ret == -1)
			return -1;
		if (read_ret == 0)
			return -1;
		readed += read_ret;
	}
	readed = 0;
	while (readed < sizeof(int))
	{
		read_ret = read(desc, (void*)len + readed, sizeof(int) - readed);
		if (read_ret == -1)
			return -1;
		if (read_ret == 0)
			return -1;
		readed += read_ret;
	}
	str = malloc((unsigned int)*len);
	readed = 0;
	while (readed < *len)
	{
		read_ret = read(desc, str + readed, *len - readed);
		if (read_ret == -1)
			return -1;
		if (read_ret == 0)
			break;
		readed += read_ret;
	}
	*arg = str;
	if (read_ret == 0)
	{
		printf("Connection lost");
		return 1;
	}
	return 0; 
}
