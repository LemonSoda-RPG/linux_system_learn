#include <stdio.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <stdlib.h>
#include "proto.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
# define IPSTRSIZE 40
#define BUFSIZE 512


static void server_job(int sd)
{
    char buf[BUFSIZE];
    int len = sprintf(buf,FMT_STAMP,(long long)time(NULL));
    if(send(sd,buf,len,0)<0)
    {
        perror("");
        exit(1);
    }
    // write(sd,buf,sizeof(buf));

}
int main()
{
    char ipstr[IPSTRSIZE];
    //需要有两个套接字  
    //一个用来监听 一个用来发送信息
    int serve_sk,client_sk;
    serve_sk = socket(AF_INET,SOCK_STREAM,0);
    if(serve_sk<0)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server_addr;
    struct sockaddr_in clent_addr;




    int val = 1;
    if(setsockopt(serve_sk,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0){
        perror("setsockopt()");
        exit(1);
    }


    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(serve_sk,"0.0.0.0",&server_addr.sin_addr);
        //监听套接字
    bind(serve_sk,(void*)&server_addr,sizeof(server_addr));
    

    if(listen(serve_sk,1024)<0)
    {
        perror("listen");
        exit(1);
    }

    socklen_t clent_len;
    clent_len = sizeof(clent_addr);
    while(1)
    {
        client_sk = accept(serve_sk,(void*)&clent_addr,&clent_len);
        if(client_sk<0)
        {
            if(errno==EAGAIN)
                continue;
            perror("accept");
            exit(1);
        }

        int pid = fork();
        if(pid<0)
        {
            perror("fork()");
            exit(1);
        }
        else if(pid == 0)
        {
            close(serve_sk);
            inet_ntop(AF_INET,&clent_addr.sin_addr,ipstr,IPSTRSIZE);
            printf("client:%s:%d\n",ipstr,ntohs(clent_addr.sin_port));
            server_job(client_sk);
            close(client_sk);
            exit(0);
        }
        /*
        这里多进程遇到的问题  和之前遇到的一样  
        就是通过send函数发送数据没有关闭套接字的话，接收方收不到消息
        然后为什么这里也会出现这样的问题呢
        client_sk = accept  这句函数出现在我们fork之前，因此在子进程中
        client_sk也被复制了一份，因此即使我们在子进程中关闭了，但是父进程
        里没有关闭，也相当于没有关闭，因此我们要在父进程中也关闭
        同理 serve_sk 父子进程也各有一个，因此我们要将子进程中的serve_sk
        关闭，保留父进程当中的
        */
        else{
            close(client_sk);
        }
    }
    close(serve_sk);
    waitpid(0,NULL,WNOHANG);  //等待给任意数量的子进程收尸
    exit(0);
}