#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "relay.h"
#define TTY1 "/dev/tty9"
#define TTY2 "/dev/tty10"
#define TTY3 "/dev/tty11"
#define TTY4 "/dev/tty12"


int main()
{
    int fd1,fd2,fd3,fd4;
    fd1 = open(TTY1,O_NONBLOCK|O_RDWR);
    if(fd1<0)
    {
        perror("open()");
        exit(1);   
        //也可以在这里判断是什么出错如果是信号打断出错
        //重复open
    }


    fd2 = open(TTY2,O_NONBLOCK|O_RDWR);
    if(fd2<0)
    {
        perror("open()");
        exit(1);   
        //也可以在这里判断是什么出错如果是信号打断出错
        //重复open
    }
    fd3 = open(TTY3,O_NONBLOCK|O_RDWR);
    if(fd3<0)
    {
        perror("open()");
        exit(1);   
        //也可以在这里判断是什么出错如果是信号打断出错
        //重复open
    }


    fd4 = open(TTY4,O_NONBLOCK|O_RDWR);
    if(fd4<0)
    {
        perror("open()");
        exit(1);   
        //也可以在这里判断是什么出错如果是信号打断出错
        //重复open
    }



    int job1 = add_job(fd1,fd2);
    if(job1<0)
    {
        perror("job1");
    }
    int job2 = add_job(fd3,fd4);
    if(job2<0)
    {
        perror("job2");
    }

    // printf("断点1\n");
    write(fd1,"TTY1\n",5);
    write(fd2,"TTY2\n",5);
    write(fd3,"TTY3\n",5);
    write(fd4,"TTY4\n",5);

    while(1)
        pause();
    // relay(fd1,fd2);


    close(fd4);
    close(fd3);
    close(fd2);
    close(fd1);
    exit(0);
}