#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
/*

*/
int main()
{
    puts("begin");
    fflush(NULL);
    int pid = fork();
    if(pid<0)
    {
        perror("fork!");
        exit(1);
    }
    if(pid==0)
    {
        // 子进程
        fflush(NULL);
        sleep(10);
        execl("/usr/sbin/date","date","+%s",NULL);
        perror("execl");
        exit(1);
    }
    sleep(15);   //如果设置的时间都是10的话  其实父进程与子进程是同时进行等待的。如果想看效果的话 这里应该设置大一点
    wait(NULL);
    
    puts("end");
    exit(0);
}