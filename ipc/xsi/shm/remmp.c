#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <aio.h>
#include <wait.h>
#define MEMSIZE 1024
int main()

{   char *ptr;
    // ftok(); //非亲缘关系的进程需要设置唯一的k值 进行通信 
    pid_t pid;
    int shmid = shmget(IPC_PRIVATE,MEMSIZE,0600);
    if(shmid<0)
    {
        perror("");
        exit(1);
    }
    pid = fork();
    if(pid<0)
    {
        perror("");
        exit(1);
    }
    if(pid==0)
    {
        
        ptr = shmat(shmid,NULL,0);
        if(ptr==(void*)-1)
        {
            perror("");
            exit(1);
        }
        strcpy(ptr,"hello");
        shmdt(ptr);
        exit(0);
    }
    else{
        wait(NULL);
        ptr = shmat(shmid,NULL,0);
        if(ptr==(void*)-1)
        {
            perror("");
            exit(1);
        }
        puts(ptr);
        shmdt(ptr);
        shmctl(shmid,IPC_RMID,NULL);
       
        exit(0);
    }

   

}