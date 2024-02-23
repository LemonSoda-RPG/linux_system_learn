#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
int main()
{
    pid_t pid = getpid();
    printf("当前进程的pid:%d\n",pid);
    pid_t ppid = getppid();
    printf("当前进程的父进程的pid:%d\n",ppid);
    
    printf("continue\n");
  
    return 0;
}
