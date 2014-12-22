#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#define PORTNUM 80
#define BUFLEN 2000

int main(int argc,char **argv)
{
    struct sockaddr_in party_addr;
    int sockfd;
    struct hostent *he;
    char buf[BUFLEN];
    int len = 0;

    if ((sockfd = socket(PF_INET,SOCK_STREAM,0))<0)
    {
        printf("cat't create socket\n");
        return 0;
    }

    memset(&party_addr, 0 , sizeof(party_addr));
    he = gethostbyname(argv[1]);
    memcpy(&party_addr.sin_addr, he->h_addr_list[0], he->h_length);
    party_addr.sin_family  = AF_INET;
    party_addr.sin_port = htons(PORTNUM);

    if ((connect(sockfd, (struct sockaddr *)&party_addr, sizeof(party_addr)))<0)
    {
        perror("error accepting connection!\n");
        return 0;
    }

    
    if (send(sockfd,argv[2],strlen(argv[2]),0)<0)
    {
        printf("error sending socket!\n");
        return 0;
    }

    while(1)
    {
    	if ((len == recv(sockfd,&buf,BUFLEN-1,0))<0)
    	{
        	printf("error reading socket!\n");
        	return 0;
    	}
    
    	printf("received : %s \n", buf);

    	if (len != BUFLEN)
    		break;
    }

    shutdown(sockfd,2);
    close(sockfd);

}
