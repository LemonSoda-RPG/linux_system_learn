#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main(int argc,char *argv[])
{
    pid_t pid;
    if(argc<3)
    {
        perror("参数数量过少");
        exit(1);
    }
    pid=fork();
    if(pid<0)
    {
        perror("fork");
        exit(1);
    }
    else if(pid==0)
    {
        setuid(atoi(argv[1]));
        execvp(argv[2],argv+2);
        perror("execvp");
        exit(1);
    }
    else
    {
        wait(NULL);
    }
    exit(0);
}