#include <stdio.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <stdlib.h>
#include "proto.h"
#include <errno.h>
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
        inet_ntop(AF_INET,&clent_addr.sin_addr,ipstr,IPSTRSIZE);
        printf("client:%s:%d\n",ipstr,ntohs(clent_addr.sin_port));
        server_job(client_sk);
        // close(client_sk);

    }
    exit(0);
}