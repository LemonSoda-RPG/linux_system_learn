#include <stdio.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proto.h"
#include <netinet/in.h> 
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/mman.h>

/*
    有两种进程  忙碌  空闲
    空闲进程的最小数量 为5  最大为10
    空闲与忙碌进程相加不能超过20个
*/
#define MINISPARESERVER 5
#define MAXSPARESERVER  10  
#define MAXCLIENT       20
#define SIG_NOTIFY      SIGUSR2
#define LINEBUFSIZE     1024
#define IPSTRSIZE 40
enum{
    STATE_IDEL = 0,
    STATE_BUSY
};
struct server_st
{
    pid_t pid;
    int state;
};
static int sock_local;
static struct server_st *serverpool;
static int idle_count;
static int busy_count;
static void* sr2_handler(int s)
{
    return NULL;
}
static int del_1_server(void)
{
    if(idle_count==0)
    {
        return -1;
    }
    int index;
    for(index=0;index<MAXCLIENT;index++)
    {
        if(serverpool[index].pid!=-1&&serverpool[index].state==STATE_IDEL)
        {
            kill(serverpool[index].pid,SIGTERM);
            serverpool[index].pid = -1;
            idle_count--;
        }
    }
}
static void server_job(int index)
{   
    socklen_t lenr;
    struct sockaddr_in addrremote;
    int sock_remote;
    char *linebuf;
    char ipstr[IPSTRSIZE];
    int ppid = getppid();
    while(1)
    {
        serverpool[index].state = STATE_IDEL;
        kill(ppid,SIG_NOTIFY);
        sock_remote = accept(sock_local,&addrremote,&lenr);  
        //这里是阻塞，可能被信号打断。如果出错，判断是不是被信号打断的
        if(sock_remote<0)
        {
            if(errno!=EINTR||errno!=EAGAIN)
            {
                //说明是真的出错了
                perror("accept()");
                exit(1);
            }
        }
        serverpool[index].state = STATE_BUSY;
        kill(ppid,SIG_NOTIFY);
        inet_ntop(AF_INET,&addrremote.sin_addr,ipstr,IPSTRSIZE);
        // printf("[%d]client:%s:%d",getpid(),ipstr,ntohs(addrremote.sin_port));
        long long stamp = time(NULL);
        int len = snprintf(linebuf,(size_t)LINEBUFSIZE,FMT_STAMP,stamp);
        send(sock_remote,linebuf,len,0);
        sleep(5);
        close(sock_remote);




    }


}
static void add_1_server(void)
{
    if(idle_count+busy_count>=MAXCLIENT)
    {
        return -1;
    }
    int index;
    for(index=0;index<MAXCLIENT;index++)
    {
        if(serverpool[index].pid == -1)
        {
            break;
        }
    }

    serverpool[index].state = STATE_IDEL;
    pid_t pid = fork();
    if(pid<0)
    {
        perror("fork()");
        exit(1);
    }
    if(pid==0)
    {
        server_job(index);
        exit(0);
    }
    else
    {
        serverpool[index].pid = pid;
        idle_count ++;
    }



}

static void scan_pool(void)
{
    int idle = 0,busy=0;
    for(int i=0;i<MAXCLIENT;i++)
    {
        if(serverpool[i].pid==-1)
        {
            continue;
        }
        if(kill(serverpool[i].pid,0))
        {
            serverpool[i].pid = -1;
            continue;
        }
        if(serverpool[i].state == STATE_IDEL)
        {
            idle++;
        }
        else if(serverpool[i].state ==STATE_BUSY)
        {
            busy++;
        }
        else
        {
            fprintf(stderr,"unknow states.\n");
            abort();
        }
    }
    idle_count = idle;
    busy_count = busy;

}




int main()
{

    
    struct sigaction sa,oldsa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;
    sa.sa_handler = SIG_IGN;  //SIGCHLD当子进程终止时，会发出这个新号
    sigaction(SIGCHLD,&sa,&oldsa);  //意思是  这个信号就是告诉不需要收尸

    sa.sa_handler = sr2_handler;
    sigemptyset(&sa.sa_mask);  //设置在调用信号处理函数时，要屏蔽的信号集合，以免函数受到干扰
    sa.sa_flags = 0;
    sigaction(SIG_NOTIFY,&sa,&oldsa);  //sig_notify信号是自定义信号，可以通过kill向指定进程号发送该信号

    sigset_t set,oset;
    sigemptyset(&set);  // 将信号集置为空
    sigaddset(&set,SIG_NOTIFY); //将自定义新号添加到信号集合当中
    sigprocmask(SIG_BLOCK,&set,&oset);  // 将信号集合中的信号设置为block



    serverpool = mmap(NULL,sizeof(struct server_st) * MAXCLIENT, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(serverpool == MAP_FAILED)
    {
        perror("mmap()");
        exit(1);    
    }
    for(int i=0;i<MAXCLIENT;i++)
    {
        serverpool[i].pid = -1;
    }





    int val = 1;
    struct sockaddr_in addrlocal;
    

    sock_local = socket(AF_INET,SOCK_STREAM,0);
    if(sock_local<0)
    {
        perror("socket");
        exit(1);
    }
    
    if(setsockopt(sock_local,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val))<0)
    {
        perror("setsockopt()");
        exit(1);
    }
    
    addrlocal.sin_family = AF_INET;
    addrlocal.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"0.0.0.0",&addrlocal.sin_addr);
    if(bind(sock_local,(struct sockaddr*)&addrlocal,sizeof(addrlocal))<0)
    {
        perror("bind()");
        exit(1);
    }
    if(listen(sock_local,100)<0)
    {
        perror("listen()");
        exit(1);
    }



    for(int i=0;i<MINISPARESERVER;i++)
    {
        add_1_server();
    }
    while(1)
    {
        sigsuspend(&oset);  //在执行到这句之前，我们的自定义信号SIG_NOTIFY被我们设置成了阻塞，运行这句话之后，函数会停止于此，阻塞。
                            //同时，会恢复到oset状态，是临时恢复，在我们的oset中，SIG_NOTIFY是unblock状态，然后我们就可以通过向函数发送
                            //SIG_NOTIFY信号，使函数取消阻塞，继续执行，同时SIG_NOTIFY被再次设置为阻塞
        scan_pool();
        //打印当前池的状态并进行管理
        if(idle_count > MAXSPARESERVER)  //如果空闲的比最大的空闲的还要多 那就删除空闲的
        {
            for(int i=0;i<(idle_count - MAXSPARESERVER);i++)
            {
                del_1_server();

            }
        }
        else if(idle_count<MINISPARESERVER)  //如果空闲的比最小的要少，那就在创建一个线程
        {
            for(int i=0;i<(MINISPARESERVER-idle_count);i++)
            {
                add_1_server();
            }
        }
        for(int i = 0;i<MAXCLIENT;i++)
        {
            if(serverpool[i].pid>0)
            {
                //打印进程状态
            }
        }

    }





    sigprocmask(SIG_SETMASK,&oset,NULL);  //恢复之前的信号


}