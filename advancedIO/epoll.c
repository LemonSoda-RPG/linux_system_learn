/*
这是一个忙等的程序
体现在如果我们一直读不到内容 就会一直返回-1
不断的假错
*/
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
// #define TTY1 "1.txt"
// #define TTY2 "2.txt"
#define BUFFERSIZE 900

enum{
    STATE_R = 1,
    STATE_W,
    STATE_AUTO, 
    STATE_EX,
    STATE_T,
};


struct fsm_st{
    int state;
    int sfd;
    int dfd;
    int len;
    int pos;
    char buffer[BUFFERSIZE];
    char *errstr;
};
static void fsm_drive(struct fsm_st* fsm)
{
    int ret = 0;
    switch(fsm->state)
    {
        case STATE_R:
            fsm->len = read(fsm->sfd,fsm->buffer,BUFFERSIZE);
            
            if(fsm->len==0)
            {
                fsm->state = STATE_T;
            }
            else if(fsm->len<0)
            {
                if(errno==EAGAIN)
                    fsm->state = STATE_R;
                else
                {
                    fsm->errstr = "readerr";
                    fsm->state = STATE_EX;
                }
            }
            else
            {
                fsm->pos = 0;
                fsm->state = STATE_W;
            }
            break;
        case STATE_W:
            
            ret = write(fsm->dfd,fsm->buffer+fsm->pos,fsm->len);
            // printf("ret:%d\n",ret);
            if(ret<0)
            {
                if(errno ==  EAGAIN)
                {
                    fsm->state=STATE_W;
                }
                else
                {
                    fsm->errstr = "writeerr";
                    fsm->state=STATE_EX;
                }
            }
            else 
            {
                fsm->pos+=ret;
                fsm->len-=ret;
                if(fsm->len==0)
                    fsm->state=STATE_R;
                else
                    fsm->state = STATE_W;    
            }
            break;
        case STATE_EX:
            perror(fsm->errstr);
            
            fsm->state = STATE_T;
            break;
        case STATE_T:
            break;
        default:
            abort();
            break;
    }
}
static int max(int fd1,int fd2)
{   
    if(fd1>=fd2)
    return fd1;
    else
    return fd2;
}
void relay(int fd1,int fd2)
{
    
    struct fsm_st fsm12,fsm21;
    struct epoll_event ev;
    memset(fsm12.buffer, 0, sizeof(fsm12.buffer));
    
    memset(fsm21.buffer, 0, sizeof(fsm21.buffer));

   

    int fd1_save = fcntl(fd1,F_GETFL);
    int fd2_save = fcntl(fd2,F_GETFL);
    fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);
    fcntl(fd2,F_SETFL,fd2_save|O_NONBLOCK);
    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_R;
    fsm21.dfd = fd1;
    fsm21.sfd = fd2;
    // printf("断点2\n");

    int epofd = epoll_create(10);
    if(epofd<0)
    {
        perror("epo_create");
        exit(1);
    }
    ev.events = 0;
    ev.data.fd = fd1;
    epoll_ctl(epofd,EPOLL_CTL_ADD,fd1,&ev);
    
    ev.events = 0;
    ev.data.fd = fd2;
    epoll_ctl(epofd,EPOLL_CTL_ADD,fd2,&ev);
    while(fsm12.state != STATE_T || fsm21.state != STATE_T)
    {
        // 布置监视任务
        ev.events = 0;
        ev.data.fd = fd1;
        if(fsm12.state==STATE_R)
        {   
           ev.events |= EPOLLIN;
        }
        if(fsm21.state==STATE_W)
        {
           ev.events |= EPOLLOUT;
        }
        epoll_ctl(epofd,EPOLL_CTL_MOD,fd1,&ev);
        
        ev.events = 0;
        ev.data.fd = fd2;

        if(fsm12.state==STATE_W)
        {
            ev.events |= EPOLLOUT;       
        }
        if(fsm21.state==STATE_R)
        {
            ev.events |= EPOLLIN;
        }
        epoll_ctl(epofd,EPOLL_CTL_MOD,fd2,&ev);

        // 监视
        if(fsm12.state < STATE_AUTO ||fsm21.state<STATE_AUTO)
        {   


            while(epoll_wait(epofd,&ev,1,-1)<0)  //1表示只要发生了一个事件那就返回
            {
                if(errno == EINTR)
                {
                    continue;
                }
                perror("poll");
                exit(1);
            }

        // 查看监视结果
            if(ev.data.fd==fd1&&ev.events&&EPOLLIN ||ev.data.fd==fd2&&ev.events&&EPOLLOUT)
                fsm_drive(&fsm12);
            
            if(ev.data.fd==fd2&&ev.events&&EPOLLIN ||ev.data.fd==fd1&&ev.events&&EPOLLOUT)
                fsm_drive(&fsm21);

        }
    // printf("断点3\n");
    }  
    close(epofd);
    fcntl(fd1,F_SETFL,fd1_save);
    fcntl(fd2,F_SETFL,fd2_save);
}


int main()
{
    int fd1,fd2;
    fd1 = open(TTY1,O_RDWR);
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

    // printf("断点1\n");
    write(fd1,"TTY1\n",5);
    write(fd2,"TTY2\n",5);
    relay(fd1,fd2);
    close(fd2);
    close(fd1);
    exit(0);
}