#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include <unistd.h>
#include <getopt.h>
#include <net/if.h>
#include <sys/types.h>       
#include <arpa/inet.h>  
#include <sys/socket.h>
#include <netinet/in.h> 
#include <string.h>
#include <errno.h>
#include "proto.h"
#define BUFSIZE 320*1024/8*20  //能够存储20秒的音乐
/*
    -M --mgroup  指定多播放组
    -P --port   指定端口
    -p --player 指定播放器
    -H --help 显示帮助
*/
struct client_conf_st client_conf = {
            .mgroup = DEFAULT_MGROUP,
            .rcvport = DEFAULT_PCVPORT,
            .player_cmd = DEFAULT_PLAYER_CMD
                   };
static ssize_t writen(int fd, const void *buf, size_t len)
{
    int ret;
    int pos = 0;
    while(len>0)
    {
        ret = write(fd,buf+pos,len);
        if(ret<0)
        {
            if(errno==EINTR)
                continue;
            perror("write");
            return -1;
            // exit(1);
        }
        len-=ret;
        pos+=ret;
    }
    return pos;


}
int main(int argc,char **argv)
{
/*
初始化 
级别：默认值，配置文件，环境变量 命令行参数
参数优先级 命令行参数最优先

*/  
    // 建立管道进行父子进程通信
    int pd[2]; 
    int ret = 0;
    int choseid;
    socklen_t len;
    int pid;
    int sd_local;
    // 接收数据缓冲区
    uint64_t receive_buf_size = BUFSIZE;
    int c=0;


    struct ip_mreqn mreq;
    int index = 0;
    //对长参数进行设置 指定是否需要传参  对于长参数 1 来判断是否需要参数
    struct option argarr[] = {{"mgroup",1,NULL,'M'},
                                {"port",1,NULL,'P'},
                                {"help",0,NULL,'H'},
                                {"player",1,NULL,'p'},
                                {NULL,0,NULL,0}};
    while(1)
    {
        //对短参数进行设置 设定是否需要传参数 对于短参数 : 来确定是否需要参数
        c = getopt_long(argc,argv,"M:P:p:H",argarr,&index);
        if(c<0)
            break;
        switch (c)
        {
            case 'P':
                client_conf.rcvport = optarg;
                break;
            case 'M':
                client_conf.mgroup = optarg;
                break;
            case 'p':
                client_conf.player_cmd = optarg;
                break;
            case 'H':
                printf("-M --mgroup  指定多播放组\n"
                        "-P --port   指定端口\n"
                        "-p --player 指定播放器\n"
                        "-H --help 显示帮助\n");
                exit(0);
            default:
                abort();
                break;
        }
    }
    

    // socket(AF_INET,SOCK_STREAM,0);

    // 创建套接字
    sd_local =  socket(AF_INET,SOCK_DGRAM,0);
    
    if(sd_local<0)
    {
        perror("socket()");
        exit(1);
    }
    
    // 将主机端转化为网络端
    inet_pton(AF_INET,client_conf.mgroup,&mreq.imr_multiaddr);

    inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);

    mreq.imr_ifindex = if_nametoindex("eth0");
    
    len = sizeof(mreq);
    // 对网络套接字进行一些属性设置

    if(setsockopt(sd_local,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,len)<0)
    {
        perror("setsockopt()");
        exit(1);
    }
    int val = 1;
    // 这个函数是干嘛的
    // 功能是 假如你是多播组中的信息发送方，通过这个设置可以使自己也能收到自己发送的数据
    if(setsockopt(sd_local,IPPROTO_IP,IP_MULTICAST_LOOP,&val,sizeof(val))<0)
    {
        perror("setsockopt()");
        exit(1);
    }

    struct sockaddr_in addrlo;
    addrlo.sin_family = AF_INET;
    addrlo.sin_port = htons(atoi(client_conf.rcvport));
    inet_pton(AF_INET,"0.0.0.0",&addrlo.sin_addr);
    socklen_t addrlen = sizeof(addrlo);
    if(bind(sd_local,(void*)&addrlo,addrlen)<0)
    {
        perror("bind()");
        exit(1);
    }


    // 创建
    if(pipe(pd)<0)   //父进程写 子进程读
    {
        perror("pipe");
        exit(1);
    }


    pid = fork();
    if(pid<0)
    {
        perror("fork()");
        exit(1);
    }
    if(pid==0)
    {
        //子进程
        close(pd[1]);
        close(sd_local);
        dup2(pd[0],0); 
        //这个时候pd[0]和0这两个文件描述符所指向的文件结构是同一个。
        //因此关闭pd[0]，我们还可以用0

        // 为什么这里要这样 是因为我们的命令只能从标准输入读取数据  所以要将我们的管道的读取接口重定向到标准输入
        if(pd[0]>0)
        {
            close(pd[0]);
        } 
        execl("/bin/sh","sh","-c",client_conf.player_cmd,NULL);
        perror("execl()");
        exit(1);
    }
    else
    {
        //父进程
        //从网络收包
        //收节目单
        //选择频道
        //收频道包
        //发送给子进程
        struct sockaddr_in raddr,raddr1;  //一个用于接收列表信息 一个用于接收频道信息
        socklen_t raddr_len,raddr_len1;
        close(pd[0]);
        struct msg_list_st *msg_list;   //频道列表信息
        msg_list = malloc(MSG_LIST_MAX);  //频道信息的最大空间
        if(msg_list==NULL)
        {
            perror("malloc");
            exit(1);
        }
        while(1)
        {
            // 持续接收数据  直到收到频道单
            len = recvfrom(sd_local,\
            msg_list,\
            MSG_LIST_MAX,\
            0,\
            (void*)&raddr,\
            &raddr_len);
            if(len<sizeof(struct msg_list_st))
            {
                fprintf(stderr,"message is to small\n");
                continue;
            }
            if(msg_list->chid!=LISTCHNID)
            {
                fprintf(stderr, "current chnid:%d.\n", msg_list->chid);
                fprintf(stderr,"chid is not match listenid\n");
                continue;
            }
            break;
        }
        //已经获得节目单
        //打印节目单信息
        // printf("打印发送方的信息：\n ip地址:%s 端口号：%d\n",inet_ntoa(raddr.sin_addr.s_addr),ntohs(raddr.sin_port));
        // printf("打印发送方的信息:\nIP地址: %s\n端口号: %d\n", 
        // inet_ntoa(*(struct in_addr*)&raddr.sin_addr.s_addr),
        // ntohs(raddr.sin_port));

        
        struct msg_listentry_st *pos;


/**
 * struct msg_list_st
    {
        //节目单
        chid_t chid;
        struct msg_listentry_st entry[1]; 
    }__attribute__((packed));
 */
        // 一开始指针指向 struct msg_listentry_st entry  
        // 直到读取到节目单的尾部为止   len就是节目单数据的总大小
        // pose每次位移 pos->len 个大小 这个大小应该是每个频道的信息的总大小
        // 对节目单进行遍历
        for(pos = msg_list->entry; (char*)pos<((char*)msg_list+len);pos=(void*)(((char*)pos)+ntohs(pos->len)))
        {
            printf("channel %d:%s\n",pos->chid,pos->desc);
        }
        // 用完我们就可以把内存释放了
        free(msg_list);
        while(ret<1)
        {
            printf("请输入序号：\n");
            ret = scanf("%d",&choseid);
            if(ret!=1)
            {
                exit(1);
            }
        }
        // 创建接收用数据包
        struct msg_channel_st *msg_channel;
        msg_channel = malloc(MSG_CHANNEL_MAX);
        if(msg_channel==NULL)
        {
            perror("malloc");
            exit(1);
        }

        // 接收缓冲区
        char rcvbuf[BUFSIZE];
        uint32_t offset = 0;
        memset(rcvbuf, 0, BUFSIZE);
        int bufct = 0; // buffer count


        while(1)
        {

            // 这里是接收所有的数据包  后面的代码会对我们选择的频道进行过滤
            len = recvfrom(sd_local,
                            msg_channel,
                            MSG_CHANNEL_MAX,
                            0,
                            (void*)&raddr1,
                            &raddr_len1);

            // 对接收到的信息进行判断  
            // 对地址进行判断
            if(raddr1.sin_addr.s_addr!=raddr.sin_addr.s_addr||raddr1.sin_port!=raddr.sin_port)
            {
                fprintf(stderr,"ignore:address not match\n");
                continue;
            }
            // struct msg_channel_st  我们定义的这个结构体的大小就是就是包的最小值了  因为里面不含数据
            // 假如比这个还小  那么一定是出错了
            if(len<sizeof(struct msg_channel_st))
            {
                fprintf(stderr,"ignore:message too small\n");
                continue;
            }
            if(msg_channel->chid==choseid)
            {   
                fprintf(stdout,"accepted msg:%d recieved.\n",msg_channel->chid);
                //坚持写够len-sizeof(chid_t)个字节

                // 将数据写入到我们的缓冲区
                // 参数  写入的起始地址  写入的数据    要写入的数据的长度 len是总长度 减去chid的长度就是数据的长度
                memcpy(rcvbuf + offset, msg_channel->data, len - sizeof(chid_t));
                offset += (len - sizeof(chid_t));



                // 为了能够流畅的播放  服务端应该发送数据适当快一些   1.1或者1.2倍
                if (bufct++ % 2 == 0){
                    // 写缓冲 每次攒够两次才会进行写入

                    // 写够offset个字节
                    if(writen(pd[1],rcvbuf,offset)<0)
                    {
                        printf("读取数据错误 结束\n");
                        exit(1);
                    }
                    offset = 0;
                }
            }
        }

        free(msg_channel);
        close(pd[1]);
        close(sd_local);
        exit(0);
    }

}