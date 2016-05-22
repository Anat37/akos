#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>

#define REQUEST 1
#define ACCEPT_CL 2
#define REFUSE 101
#define ACCEPT 102
#define STRING 103

typedef struct {
    long mes_type;
    int a, b;
    long result;
    pid_t client;
} mes_t;

key_t key;
int msgid;
mes_t data;
pid_t currentcl = 0;

void handler(int sig)
{
    if (sig == SIGINT || sig == SIGHUP)
    {
        msgctl(msgid, IPC_RMID, NULL);
        exit(0);
    }
}

int main()
{
    FILE* fstr;
    char* str;
    fstr = fdopen(0, "r");
    key = ftok("/bin/ls", '1');
    msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0600);

    if (msgid == -1) {
        printf("msgget failed\n");
        return 1;
    }

    signal(SIGINT, handler);
    signal(SIGHUP, handler);

    while (1) {
        msgrcv(msgid, &data, sizeof(mes_t) - sizeof(long), -100, 0);   
        
        if (data.mes_type == REQUEST || currentcl != 0)
        {
        	data.mes_type = REFUSE + (long)data.client;
        	msgsnd(msgid, &data, sizeof(mes_t) - sizeof(long), 0);
        }
        
        if (data.mes_type == REQUEST || currentcl == 0)
        {	
        	currentcl = (long)data.client;
        	data.mes_type = ACCEPT + (long)data.client;
        	msgsnd(msgid, &data, sizeof(mes_t) - sizeof(long), 0);
        } 
        
        if (data.mes_type == ACCEPT_CL || currentcl != 0)
        {	
        	safe_gets(fstr, &str);
        	*((long*)str) = (STRING + (long)data.client);
        	msgsnd(msgid, &str, sizeof(mes_t) - sizeof(long), 0);
        } 
    }
    return 0;
}
