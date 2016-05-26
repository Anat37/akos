#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>

#define REQUEST 1
#define RESPONSE 2

typedef struct {
    long mes_type;
    int a, b;
    long result;
    pid_t client;
} mes_t;

key_t key;
int msgid;
mes_t data;
pid_t pid;

void handler(int sig)
{
    if (sig == SIGINT)
    {
        exit(0);
    }
    signal(SIGINT, handler);
}

int main()
{
    key = ftok("/bin/ls", '1');
    msgid = msgget(key, 0600);

    if (msgid == -1) {
        printf("msgget failed\n");
        return 1;
    }
    signal(SIGINT, &handler);
    data.a = rand();
    data.b = rand();	
    pid = getpid();
    data.client = pid;
    data.mes_type = REQUEST;
    msgsnd(msgid, &data, sizeof(mes_t) - sizeof(long), 0);

    msgrcv(msgid, &data, sizeof(mes_t) - sizeof(long), RESPONSE + (long)pid, 0);   
    
    printf("Get from server:%d, %d, %ld\n", data.a, data.b, data.result);
    fflush(stdout);
    
    return 0;
}
