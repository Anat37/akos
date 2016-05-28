#include "header.h"
#include <math.h>
unsigned daemon_flag = 0;



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
  free(mobjects);
	return 0;
}

#include "serverpart.h"
#include "daemonbeing.h"


void map_read()
{
	FILE* mapf = fopen(map_file_name, "r");
	int i;
	char* str = NULL;
	float value;
	int itemsize = 0;
	map_name = (char*) malloc(256 * sizeof(char)); 
	if (mapf == NULL)
  {
    err_report("Bad map file", 0);
    exit(0);
  }
  
	fscanf(mapf, "%s %dx%d", map_name, &map_size_n, &map_size_m);
  
  map_size_n += 2;
  map_size_m += 2;
	map = (char**) malloc((map_size_n) * sizeof(char*));
	fscanf(mapf, "\n");
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
  printf("initial_health set to %f\n",initial_health);
  
  fscanf(mapf, "%s = %f", str, &hit_value);
  printf("hit_value set to %f\n",hit_value);
  
	fscanf(mapf, "%s = %f", str, &value);
  printf("recharge_duration set to %f\n",value);
  recharge_duration = roundf(value*1000);
  
  
	fscanf(mapf, "%s = %f", str, &value);
	mining_time = roundf(value*1000);
  printf("mining_time set to %f\n",value);
  
	fscanf(mapf, "%s = %f", str, &stay_health_drop);
  printf("stay_health_drop set to %f\n",stay_health_drop);
  
	fscanf(mapf, "%s = %f", str, &movement_health_drop);
	printf("movement_health_drop set to %f\n",movement_health_drop);
  
  fscanf(mapf, "%s = %f", str, &value);
	step_standard_delay  = roundf(value*1000);
	printf("step_standard_delay set to %d\n",step_standard_delay);
  
  
  fscanf(mapf, "%s = %f", str, &value);
	moratory_duration =  roundf(value*1000);
	printf("moratory_duration set to %f\n",value);
  fscanf(mapf, "\nitems:\n");
  
  
	free(str);
	while (!feof(mapf))
	{
		if (itemsize <= mobjectscnt)
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
void init_var(){
	team_arr_size = 0;
	player_arr_size = 0;

	port_num = DEF_PORT;
	map_file_name = "defmap.map";

	mobjects= NULL;
	mobjectscnt =  0;
  team_arr = NULL;
  player_arr = NULL; 
  sthreads = NULL;
  plthread = NULL;
}
int main(int argc, char* argv[])
{	
	init_var();
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
