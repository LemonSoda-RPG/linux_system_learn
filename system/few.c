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
        execl("/usr/sbin/date","date","+%s",NULL);
        perror("execl");
        exit(1);
    }

    wait(NULL);
    puts("end");
    exit(0);
}