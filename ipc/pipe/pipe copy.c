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
        printf("子进程 %d   %d\n",pd[0],pd[1]);
        //子进程 读管道
        close(pd[1]);
        close(pd[0]);
        // int len = read(pd[0],buffer,2);  //如果当前没有数据，只要有一个写端没有关闭，那么就等待，进入阻塞
                                          //如果当前有数据，不管当前有没有写端，都正常读取，不阻塞
                                          // 只有当读的时候才会进行判断
    
        // close(pd[0]);
        exit(0);

    }
    else
    {   
        close(pd[0]);
        sleep(2);
        // printf(" 父进程  %d   %d\n",pd[0],pd[1]);
        //父进程  写管道
        // close(pd[0]);
        write(pd[1],"hello!",6);
        // close(pd[1]);
        wait(NULL);
        exit(0);
    }
    
    
    
    }