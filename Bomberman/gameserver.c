#include "header.h"

unsigned daemon_flag = 0;
unsigned port_num = 1050;
char* map_file_name = "defmap.map";
char* map_name;
char** map;
int map_size_n;
int map_size_m;
float initial_health = 500;
float hit_value      = 50;
float recharge_duration = 3;
float mining_time = 3;
float stay_health_drop = 1;
float movement_health_drop = 4;
float step_standard_delay = 0.1;
float moratory_duration = 5;

void help_print()
{
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
	return 0;
}

#include "serverpart.h"
#include "daemonbeing.h"

void map_read()
{
	FILE* mapf = fopen(map_file_name, "r");
	int i;
	char* str = NULL;
	
	map_name = (char*) malloc(256 * sizeof(char)); 
	
	fscanf(mapf, "%s %dx%d", map_name, &map_size_n, &map_size_m);
	map = (char**) malloc((map_size_n + 2) * sizeof(char*));
	
	for (i = 0; i < map_size_n; i++)
	{
		str = NULL;
		printf("ok\n");
		safe_gets(mapf, &str);
		map[i] = str; 
	}
	str = NULL;
	str = (char*) malloc(256 * sizeof(char));
	fscanf(mapf, "\n"); 
	fscanf(mapf, "%s = %f", str, &initial_health);
	fscanf(mapf, "%s = %f", str, &hit_value);
	fscanf(mapf, "%s = %f", str, &recharge_duration);
	fscanf(mapf, "%s = %f", str, &mining_time);
	fscanf(mapf, "%s = %f", str, &stay_health_drop);
	fscanf(mapf, "%s = %f", str, &movement_health_drop);
	fscanf(mapf, "%s = %f", str, &step_standard_delay);
	fscanf(mapf, "%s = %f", str, &moratory_duration);
	free(str);
	fclose(mapf);
}

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
    				if (port_num > 10000)
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
	printf("%f\n", mining_time);
	return stop_server();
}
