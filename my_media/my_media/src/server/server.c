#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <features.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <signal.h>
#include "medialib.h"
#include "proto.h"
#include "site_type.h"
#include "server_conf.h"
#include "thr_list.h"
#include "thr_channel.h"
int serversd;
static struct mlib_listentry_st *list;
struct server_conf_st server_default_conf = {
    .rcvport = DEFAULT_PCVPORT,
    .mgroup = DEFAULT_MGROUP,
    .media_dir = DEFAULT_MEDIADIR,
    .runmode = RUN_DAEMON,
    .ifname = DEFAULT_IF
};
struct sockaddr_in sndaddr;
static int daemonzize()
{
    pid_t pid = fork();
    if(pid<0)
    {
        // perror("fork()");
        syslog(LOG_ERR,"fork() failed");

        return -1;
    }
    if(pid==0)
    {
        int fd = open("/dev/null",O_RDWR);
        if(fd<0)
        {
            syslog(LOG_WARNING,"open() failed");
        }
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        if(fd>2)
            close(fd);
        setsid();
        chdir("/");
        umask(0);
        return 0;
    }
    if(pid>0)
    {
        exit(0);
    }
}
static void daemon_exit(int s)
{

    // 退出  销毁所有
    thr_list_destory();
    thr_channel_destoryall();
    mlib_freechnlist(list);
    closelog();
    exit(0);
}
static int socket_init(void)
{
    struct ip_mreqn mreq;
    // int val = 1;
    
    serversd = socket(AF_INET,SOCK_DGRAM,0);
    if(serversd<0)
    {
        syslog(LOG_ERR,"socket");
        exit(1);
    }
    // 这是创建多播组的参数
    inet_pton(AF_INET,server_default_conf.mgroup,&mreq.imr_multiaddr);
    inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex(server_default_conf.ifname);
    socklen_t len = sizeof(mreq);

    if(setsockopt(serversd,IPPROTO_IP,IP_MULTICAST_IF,&mreq,len)<0)
    {
        syslog(LOG_ERR,"setsocket:");
        exit(1);
    }

    // bind()
    // 这是发送目标的参数  发送到多播组 并且指定端口
    sndaddr.sin_family = AF_INET;
    sndaddr.sin_port = htons(atoi(server_default_conf.rcvport));
    inet_pton(AF_INET,server_default_conf.mgroup,&sndaddr.sin_addr);
    return 0;
}
int main(int argc,char **argv)
{

    // 命令行分析
    /*
     *   -M     指定多播组
     *   -P     指定接受端口
     *   -F     前台运行
     *   -H     显示帮助
     *   -I     指定网络设备
     *   -D     指定媒体库路径
     *
     */
    struct option argarr[] = {
            {"mgroup",1,NULL,'M'},
            {"port",1,NULL,'P'},
            {"help",0,NULL,'H'},
            {"directory",1,NULL,'D'},
            {NULL,0,NULL,0}
    };
   


    struct sigaction sa;
    
    sa.sa_handler = daemon_exit;

    // 对信号集进行清空
    sigemptyset(&sa.sa_mask);
    // 将信号添加到信号集
    // 信号集中的信号在调用信号处理函数时会被屏蔽  避免重入  重入就是信号处理函数还没有运行完成，又来了一个信号，会导致再次触发信号处理函数
    // 标准信号与实时信号
    sigaddset(&sa.sa_mask,SIGTERM);
    sigaddset(&sa.sa_mask,SIGINT);
    sigaddset(&sa.sa_mask,SIGQUIT);
    sigaction(SIGTERM,&sa,NULL);
    sigaction(SIGINT,&sa,NULL);
    sigaction(SIGQUIT,&sa,NULL);
    // 配置LOG_PERROR 意思是在输入到日志的同时 在标准错误也进行输出  但是使用守护进程就无法输出了
    openlog("netradio",LOG_PERROR|LOG_PID,LOG_DAEMON);
    while(1)
    {
        int index = 0;
        int c = getopt_long(argc,argv,"M:P:FHD:I:",argarr,&index);
        if(c<0)
        {
            break;
        }
        switch(c) {
            case 'M':
                server_default_conf.mgroup = optarg;
                break;
            case 'F':
                // 前台运行  就不改变输入输出了
                server_default_conf.runmode = RUN_FRONT;
                break;
            case 'H':
                printf("     *   -M   --mgroup    指定多播组\n"
                       "     *   -P   --port      指定接受端口\n"
                       "     *   -F               前台运行\n"
                       "     *   -H   --help      显示帮助\n"
                       "     *   -I               指定网络设备\n"
                       "     *   -D   --directory 指定媒体库路径\n");
                break;
            case 'D':
                server_default_conf.media_dir = optarg;

                break;
            case 'P':
                server_default_conf.rcvport = optarg;
//                printf("%s",server_default_conf.rcvport);
                break;
            case 'I':
                server_default_conf.ifname = optarg;
            default:
                abort();
                break;
        }

    }

    syslog(LOG_ERR,"hahahahahahahah");

    // 将服务器端设置为守护进程
    if(server_default_conf.runmode==RUN_DAEMON) {
        if(daemonzize()!=0)
        {
            exit(1);
        }
        
    }
    else if(server_default_conf.runmode==RUN_FRONT)
    {
        /*
         * do nothing
         */
    }
    else
    {
        syslog(LOG_ERR,"EINVAL server_conf");
        // fprintf(stderr,"EINVAL\n");
        exit(1);
    }

    /*
     * socket初始化
     * */
    
    socket_init();
    /*
     * 获取频道信息
     * */
    int list_size;
    int err;
    // 这里传递的是指针的指针
    err = mlib_getchnlist(&list,&list_size);
    if(err<0)
    {
        syslog(LOG_ERR,"mlib_getchnlist");
        exit(1);
    }
    /*
     *
     * 创建节目单线程
     * */
    thr_list_create(list,list_size);

    /*
     * 创建频道线程
     */
    int i = 0;
    for(i = 0;i<list_size;i++)
    {   //为什么这里可以直接传递指针呢  因为这里并不是动态分配内存
        //不对  也是动态分配内存 但是结构体占用的内存与结构体中的描述字符串占用的内存并不在同一块空间
        //因此 在这个结构体数组中，每个结构体所占用的空间大小都是一样的，因此可以通过下标进行遍历
        err = thr_channel_create(list+i);
        if(err)
        {
            syslog(LOG_WARNING,"err = thr_channel_create %d",i);
        }
    }
    syslog(LOG_DEBUG,"%d channel threads created.",i);
    

    while(1)
        pause();
    closelog();
    exit(0);
}