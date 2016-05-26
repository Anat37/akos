#include "header.h"

unsigned port_num = 1050;
char* map_file_name = "defmap.map";
char* map_name;
char** map;
struct object* mobjects= NULL;
int mobjectscnt =  0;
int map_size_n;
int map_size_m;


void err_report(const char* str, short flag)	/*flag for system call error*/
{
	if (daemon_flag)
		syslog(LOG_ERR, str);
	else if (flag)
		perror(str);
		else printf("%s\n", str);	
}

int free_mem()
{
	int i;
	for (i = 0; i < map_size_n; i++)
	{
		free(map[i]); 
	}
	free(map);
	free(map_name);
    free(objects);
	return 0;
}

#include "msg.h"
#include "serverpart.h"
#include "daemonbeing.h"