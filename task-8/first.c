#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static char *shmaddr;

void handler(int c)
{
    shmaddr[1] = 0;
    return;
}

int main()
{
    key_t key;
    int shmid,
        semid;
    struct sembuf op_in, op_out;
    
    key = ftok("/bin/bash",'c');
    shmid = shmget(key,2*sizeof(int),0666|IPC_CREAT);
    if (shmid==-1)
    {
        semctl(semid,IPC_RMID,(int)0);
        return 1;
    }
    shmaddr = shmat(shmid,NULL,0);
    semid = semget(key,3,0666|IPC_CREAT);
    if (semid==-1)
    {
        shmctl(shmid,IPC_RMID,NULL);
        return 1;
    }
    shmaddr[0] = 0;
    shmaddr[1] = 1;
    signal(SIGINT,handler);

    op_in.sem_op = -1;
    op_in.sem_num = 2;
    op_in.sem_flg = 0;

    op_out.sem_op = 2;
    op_out.sem_num = 0;
    op_out.sem_flg = 0;    
   
    semop(semid,&op_out,1);

    while(shmaddr[1])
    {
        semop(semid,&op_in,1);
        printf("%i has got %i\n",getpid(),shmaddr[0]);
        shmaddr[0] += 1;
        sleep(1);
        semop(semid,&op_in,1);
        semop(semid,&op_out,1);
    }
    
    semctl(semid,IPC_RMID,(int)0);
    shmdt(shmaddr);
    shmctl(shmid,IPC_RMID,NULL);

    printf("first ends\n");
    return 0;
}
