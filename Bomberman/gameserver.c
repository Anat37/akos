#include "serverheader.h"



int free_mem()
{
	int i;
	for (i = 0; i < map_size_n; i++)
	{
		free(map[i]); 
	}
	free(map);
	free(map_name);
	return 0;
}

void map_read()
{
	FILE* mapf = fopen(map_file_name, "r");
	int i;
	char* str = NULL;
	float value;
	int itemsize = 0;
	map_name = (char*) malloc(256 * sizeof(char)); 
	
	fscanf(mapf, "%s %dx%d", map_name, &map_size_n, &map_size_m);
	map = (char**) malloc((map_size_n + 2) * sizeof(char*));
	
	for (i = 0; i < map_size_n; i++)
	{
		str = NULL;
		safe_gets(mapf, &str);
		map[i] = str; 
	}
	str = NULL;
	str = (char*) malloc(256 * sizeof(char));
	fscanf(mapf, "\n"); 
	fscanf(mapf, "%s = %f", str, &initial_health);
	fscanf(mapf, "%s = %f", str, &hit_value);
	fscanf(mapf, "%s = %f", str, &value);
	recharge_duration 
	fscanf(mapf, "%s = %f", str, &value;);
	mining_time == roundf(value*1000);
	fscanf(mapf, "%s = %f", str, &stay_health_drop);
	fscanf(mapf, "%s = %f", str, &movement_health_drop);
	fscanf(mapf, "%s = %f", str, &value);
	step_standard_delay = = roundf(value*1000);
	fscanf(mapf, "%s = %f", str, &value);
	moratory_duration =  roundf(value*1000);
	fscanf(mapf, "\nitems:\n");
	free(str);
	while (!feof(mapf))
	{
		if (itemsize == mobjectscnt)
		{
			itemsize += 10;
			mobjects = realloc(mobjects, itemsize * sizeof(struct mobject)); 
		}
		fscanf(mapf, "%d %d %d\n", &mobjects[mobjectscnt].x,  &mobjects[mobjectscnt].y,  &mobjects[mobjectscnt].arg);
		mobjects[mobjectscnt].type = OBJ_MAP;
		++mobjectscnt;
	}
	
	fclose(mapf);
}

#include "msg.h"
#include "serverpart.h"
#include "daemonbeing.h"

void options(int argc, char* argv[])
{
	char *opts = "dl:m:p:n:h";/* daemon trigger, logname, map file, port, pid file name, help*/
    	int opt; 
    	while((opt = getopt(argc, argv, opts)) != -1)
    	{  
    		switch ((char)opt)
    		{
    			case 'd': daemon_flag = 1; 
    				break;
    			case 'l': logname = optarg;
    				break;
    			case 'm': map_file_name = optarg;
    				break;
    			case 'p': port_num = atoi(optarg);
    				if (port_num > 65000)
    					{
    						printf("Invalid port number");
    						port_num = 1050;	
    					}
    				break;
    			case 'n': pid_file_name = optarg;
    				break;
    			case 'h': help_print();
    				break;
        	}	
        }	
}

int main(int argc, char* argv[])
{	
	options(argc, argv);
	if (daemon_flag)
	{	
		if (become_daemon() != 0)
			daemon_flag = 0;
	}
	map_read();
	server_work();
	return stop_server();
}
