#include "header.h"
#include "msg.h"

unsigned port_num = 1050;
char* hostname = "localhost"; 
int sock_id;

struct hostent *hst;
struct sockaddr_in server_addr;
FILE* file;


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
	printf("ok\n");
	
    	msg_rec(sock_id, &type, &len,(void**) &buf);
    	printf("ok2\n");
    	printf("%s\n", buf);
    	
    	printf("\nEND\n");
    	free(buf);
    	close(sock_id);
	return 0;
}
