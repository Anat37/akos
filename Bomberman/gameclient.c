#include<unistd.h>

unsigned daemon_flag = 0;
unsigned port_num = 1050;
char* map_file_name = "defaultmap";
char* logname = "syslog";
char* pid_file_name = "bombermanpid"; 

void help_print()
{
}

void options(int argc, char* argv[])
{
	char *opts = "s:p:h";/* hostname, port, help*/
    	int opt; 
    	while((opt = getopt(argc, argv, opts)) != -1)
    	{  
    		switch ((char)opt)
    		{
    			case p: port_num = atoi(optarg);
    				if (port_num > 10000)
    					{
    						printf("Invalod port number");
    						port_num = 1050;	
    					}
    				break;
    			case s: 
    			case h: help_print();
    				break;
        	}	
        }	
}
