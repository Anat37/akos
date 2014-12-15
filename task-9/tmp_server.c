#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/signal.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define SADDRESS "mysocket"
#define CADDRESS "clientsocket"
#define BUFLEN 256

int status = 1;

void handler(int c)
{
    status = 0;
}

int main(int argc,char **argv)
{
    int sockid;
    unsigned int party_len;
    struct sockaddr_un party_addr, own_addr;
    char buf[BUFLEN];
    
    signal(SIGINT,handler);

    own_addr.sun_family = AF_UNIX;
    strcpy(own_addr.sun_path, SADDRESS);
    unlink(own_addr.sun_path);
    sockid = socket(AF_UNIX, SOCK_DGRAM, 0);
    bind(sockid,(struct sockaddr *)&own_addr,sizeof(own_addr.sun_family)+strlen(own_addr.sun_path)+1);
    
    while(status)
    {
        recvfrom(sockid, buf, BUFLEN, 0, (struct sockaddr*)&party_addr, &party_len);
        printf("I has got '%s'\n",buf);
        strcpy(buf,"Hello, I am server, nice to meet you\n");
        sendto(sockid, buf, strlen(buf)+1, 0, (struct sockaddr*)&party_addr,party_len);
    }

    close(sockid);
    unlink(own_addr.sun_path);
    printf("Server closed\n");
}
