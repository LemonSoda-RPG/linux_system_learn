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
    有三种进程  忙碌  空闲  未被启用的进程
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
static int idle_count=0;
static int busy_count=0;

static void sr2_handler(int s);
static int  del_1_server(void);
static void server_job(int index);
static int  add_1_server(void);
static void scan_pool(void);

int main()
{

    struct sigaction sa,oldsa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT; //  让父进程自动回收 而不需要显式的调用wait函数
    sa.sa_handler = SIG_IGN;  //SIGCHLD当子进程终止时，会发出这个信号
    sigaction(SIGCHLD,&sa,&oldsa);  //意思是  这个信号就是告诉不需要收尸

    sa.sa_handler = sr2_handler;
    sigemptyset(&sa.sa_mask);  //设置在调用信号处理函数时，要屏蔽的信号集合，以免函数受到干扰
    sa.sa_flags = 0;
    sigaction(SIG_NOTIFY,&sa,&oldsa);  //sig_notify信号是自定义信号，可以通过kill向指定进程号发送该信号

    sigset_t set,oset;
    sigemptyset(&set);  // 将信号集置为空
    sigaddset(&set,SIG_NOTIFY); //将自定义新号添加到信号集合当中
    sigprocmask(SIG_BLOCK,&set,&oset);  //将信号集合中的信号设置为block

    //创建内存匿名映射  分别是 内存起始地址（这里不需要）  所需内存大小   我们对这块内存要做的操作  内存权限  文件描述符，指定映射的文件  指针偏移量
    serverpool = mmap(NULL,sizeof(struct server_st) * MAXCLIENT, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //如果映射失败 则退出
    if(serverpool == MAP_FAILED)
    {
        perror("mmap()");
        exit(1);    
    }
    //对我们准备的线程进行初始化 将pid设置为-1
    for(int i=0;i<MAXCLIENT;i++)
    {
        serverpool[i].pid = -1;
    }


    //设置监听socket套接字

    int val = 1;
    struct sockaddr_in addrlocal;
    
    sock_local = socket(AF_INET,SOCK_STREAM,0);
    if(sock_local<0)
    {
        perror("socket");
        exit(1);
    }
    //SO_REUSEADDR选项允许套接字重新使用本地地址和端口 因为有可能连接还没有结束导致无法使用同一个端口
    //  参数分别是  套接字   设置所在的层级  进行的具体的设置 设置的开关地址起始  开关的长度 
    if(setsockopt(sock_local,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val))<0)
    {
        perror("setsockopt()");
        exit(1);
    }
    
    addrlocal.sin_family = AF_INET;
    addrlocal.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"0.0.0.0",&addrlocal.sin_addr);
    //将监听的信息绑定到这个套接字上
    if(bind(sock_local,(void*)&addrlocal,sizeof(addrlocal))<0)
    {
        perror("bind()");
        exit(1);
    }
    //将套接字设置为监听状态
    if(listen(sock_local,100)<0)
    {
        perror("listen()");
        exit(1);
    }
    // 初始化
    // 如果当前空闲的线程小于最小值5  那么我们就创建线程
    for(int i=0;i<MINISPARESERVER;i++)
    {
        add_1_server();
    }
    while(1)
    {
        //  在while循环中检查进程池的状态，并根据当前状态选择删除线程或者创建线程
        //  任何未被阻塞的信号都可以打断sigsuspend的阻塞
        sigsuspend(&oset);  //在执行到这句之前，我们的自定义信号SIG_NOTIFY被我们设置成了阻塞，运行这句话之后，函数会停止于此，阻塞。
                            //同时，会恢复到oset状态，是临时恢复，在我们的oset中，SIG_NOTIFY是unblock状态，然后我们就可以通过向函数发送
                            //SIG_NOTIFY信号，使函数取消阻塞，继续执行，同时SIG_NOTIFY被再次设置为阻塞
        scan_pool();  //获取当前进程池中的状态
        //打印当前池的状态并进行管理
        if(idle_count > MAXSPARESERVER)  //如果空闲的比最大的空闲的还要多 那就删除空闲的
        {
            for(int i=0;i<(idle_count - MAXSPARESERVER);i++)
            {
                del_1_server();
            }
        }
        else if(idle_count<MINISPARESERVER)  //如果空闲的比最小的要少，那就再创建一个线程
        {
            for(int i=0;i<(MINISPARESERVER-idle_count);i++)
            {
                add_1_server();
            }
        }
        for(int i = 0;i<MAXCLIENT;i++)
        {
            if(serverpool[i].pid==-1)
            {   
                putchar('\'');
                //打印进程状态
            }
            else if(serverpool[i].state==STATE_IDEL)
            {
                putchar('.');
            }
            else
            {
                putchar('x');
            }
        }
        putchar('\n');

    }

    sigprocmask(SIG_SETMASK,&oset,NULL);  //恢复之前的信号
    exit(0);
}
static void sr2_handler(int s)
{
    return;
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
            break;
        }
    }
    return 0;
}
static void server_job(int index)
{   
    socklen_t lenr;
    struct sockaddr_in addrremote;
    int sock_remote;
    char linebuf[LINEBUFSIZE];
    char ipstr[IPSTRSIZE];
    int ppid = getppid();
    while(1)
    {
        serverpool[index].state = STATE_IDEL;
        kill(ppid,SIG_NOTIFY);
        sock_remote = accept(sock_local,(void*)&addrremote,&lenr);  
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
        
        kill(ppid,SIG_NOTIFY);  //每次变更状态都要通知父进程遍历一次线程池的状态
        inet_ntop(AF_INET,&addrremote.sin_addr,ipstr,IPSTRSIZE);
       
        int len = sprintf(linebuf,FMT_STAMP,(long long)time(NULL));
        
        send(sock_remote,linebuf,len,0);
        sleep(5);
        close(sock_remote);
        
    }

}
static int add_1_server(void)
{
    if(idle_count+busy_count>=MAXCLIENT)
    {
        printf("进程池已经被占满，无法继续创建进程\n");
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
        //我们在serverjob里改变idleconunt和busycount是没有用的 因为这里已经是子进程了
        //在子进程里修改变量对父进程无效
        //如果我们想要使用全局变量 那么还要使用互斥锁
        server_job(index);
        exit(0);
    }
    else
    {
        serverpool[index].pid = pid;
        idle_count ++;
    }
    return 0;
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
            printf("这里有一个线程没有了，但是没有被设置为-1");
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