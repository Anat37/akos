#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <stdio.h>

int main()
{
    key_t key;
    int shmid,
        semid;
    char *shmaddr;
    struct sembuf op_in, op_out;
    
    key = ftok("/bin/bash",'c');
    shmid = shmget(key,2*sizeof(int),0666|IPC_CREAT);
    shmaddr = shmat(shmid,NULL,0);
    semid =semget(key,3,0666|IPC_CREAT);

    op_in.sem_op = -1;
    op_in.sem_num = 0;
    op_in.sem_flg = 0;

    op_out.sem_op = 2;
    op_out.sem_num = 1;
    op_out.sem_flg = 0;    

    while(shmaddr[1])
    {
        semop(semid,&op_in,1);
        printf("%i has got %i\n",getpid(),shmaddr[0]);
        shmaddr[0] += 1;
        sleep(1);
        semop(semid,&op_in,1);
        semop(semid,&op_out,1);
    }

    printf("second ends\n");
    return 0;
}
