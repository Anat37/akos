#include <pwd.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>

void help_print()
{
	printf("help is not ready yet:)");
}

unsigned daemon_flag = 0;

void err_report(char* str, short flag)	/*flag for system call error*/
{
	if (daemon_flag)
		syslog(LOG_ERR, str);
	else if (flag)
		perror(str);
		else printf("%s\n", str);	
}
