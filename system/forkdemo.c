#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
int main()
{
    pid_t pid = fork();
    if(pid==0)
    {
        printf("子进程\n");
        sleep(1);
        printf("子进程\n");
        sleep(1);
        printf("子进程\n");
        sleep(1);
        printf("子进程\n");
        sleep(1);
        printf("子进程\n");
        printf("子进程\n");
        printf("子进程\n");
        printf("子进程\n");
        printf("子进程\n");
        printf("子进程\n");
    }
    else if(pid>0)
    {
        printf("父进程\n");
        sleep(1);
        printf("父进程\n");
        sleep(1);
        printf("父进程\n");
        sleep(1);
        printf("父进程\n");
        printf("父进程\n");
        printf("父进程\n");
        printf("父进程\n");
        printf("父进程\n");
        printf("父进程\n");
            
        
    }
    return 0;
}