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

int main(int argc,char **argv)
{
    int sockid;
    struct sockaddr_un party_addr, own_addr;
    char buf[BUFLEN];

    own_addr.sun_family = AF_UNIX;
    strcpy(own_addr.sun_path, CADDRESS);
    unlink(own_addr.sun_path);
    sockid = socket(AF_UNIX, SOCK_DGRAM, 0);
    bind(sockid,(struct sockaddr *)&own_addr,sizeof(own_addr.sun_family)+strlen(own_addr.sun_path)+1);
    
    /*client*/
    
    party_addr.sun_family = AF_UNIX;
    strcpy(party_addr.sun_path, SADDRESS);
    strcpy(buf,"request");
    sendto(sockid, buf, strlen(buf)+1, 0, (struct sockaddr*)&party_addr,sizeof(party_addr.sun_family)+strlen(SADDRESS)+1);
    recvfrom(sockid, buf, BUFLEN, 0, NULL, 0);
    printf("I has got answer '%s' from server\n", buf);

    close(sockid);
    unlink(own_addr.sun_path);
    printf("Client closed\n");
}
