#include<stdio.h>
#include<stdlib.h>
#include<aio.h>
#include<unistd.h>
#include<wait.h>
#define BUFSIZE 1024
int main()
{
    int pd[2];  //pipe如果成功 会返回两个文件描述符
    char buffer[BUFSIZE];
    pid_t pid;
    if(pipe(pd)<0)
    {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if(pid<0)
    {
        perror("pid");
        exit(1);
    }


    if(pid == 0)
    {

        close(pd[1]);
        dup2(pd[0],0);
        close(pd[0]);
        execl("/usr/bin/mpg123","mpg123","-",NULL);
        perror("execl");
        exit(1);
        

    }
    else
    {
        close(pd[0]);
        // 父进程从网络接受数据并写入到管道当中
        close(pd[1]);
        wait(NULL);
        exit(0);
    }



}