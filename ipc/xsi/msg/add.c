#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#define PROCNUM 20
#define FNAME "/tmp/out"
#define LINESIZE 1024
int semid;
static void  P(void)
{
    struct sembuf op;
    op.sem_flg = 0;
    op.sem_num = 0;
    op.sem_op = -1;
    while(semop(semid,&op,1)<0)
    {
        if(errno!=EINTR&&errno!=EAGAIN)
        {
            perror("");   // 如果资源不满足当前的要求  就在这里等待
            exit(1);
        } 
    }
}
static void  V(void)
{
    struct sembuf op;
    op.sem_flg = 0;
    op.sem_num = 0;
    op.sem_op = 1;
    if(semop(semid,&op,1)<0)
    {
        perror("");   // 如果资源不满足当前的要求  就在这里等待
        exit(1);
    }

}
static void func_add(void)
{
    FILE *fp;
    int fd;
    char linebuf[LINESIZE];
    fp = fopen(FNAME,"r+");
    if(fp==NULL)
    {
        perror("fopen");
        exit(1);
    }
   
    P();

    fgets(linebuf,LINESIZE,fp);
    fseek(fp,0,SEEK_SET);
    // sleep(1);
    fprintf(fp,"%d\n",atoi(linebuf)+1);
    fflush(fp);

    V();

    fclose(fp);
    return;

}
int main()
{
    pid_t pid;
    int i;
    key_t key;

    semid = semget(IPC_PRIVATE,1,0600);
    if(semid<0)
    {
        perror("semget");
    }

    if(semctl(semid,0,SETVAL,1)<0)
    {
        perror("");
        exit(1);
    }

    for(i = 0;i<PROCNUM;i++)
    {
        pid = fork();
        if(pid<0)
        {
            perror("");
            exit(1);
        }
        if(pid==0)
        {
            func_add();
            exit(0);
        }
    }
    for(i =0;i<PROCNUM;i++)
    {
        wait(NULL);
    }

    semctl(semid,0,IPC_RMID);
    exit(0);
}