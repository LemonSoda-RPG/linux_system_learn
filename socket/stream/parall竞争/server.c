#include <stdio.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
# define IPSTRSIZE 40
#define BUFSIZE 512
#define PROCNUM 4
#define SERVERPORT "8099"
#define FMT_STAMP "%lld\r\n"

static void server_job(int sd)
{
    char buf[BUFSIZE];
    int len = sprintf(buf,FMT_STAMP,(long long)time(NULL));
    if(send(sd,buf,len,0)<0)
    {
        perror("");
        exit(1);
    }
}


static void server_loop(int serve_sk)
{   
    int client_sk;
    char ipstr[IPSTRSIZE];
    struct sockaddr_in clent_addr;
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
        printf("[%d]client:%s:%d\n",getpid(),ipstr,ntohs(clent_addr.sin_port));
        server_job(client_sk);
        close(client_sk);
        
    }
    close(serve_sk);
}
    
int main()
{
    
    //需要有两个套接字  
    //一个用来监听 一个用来发送信息
    int serve_sk;
    serve_sk = socket(AF_INET,SOCK_STREAM,0);
    if(serve_sk<0)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server_addr;

    int val = 1;
    if(setsockopt(serve_sk,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0){
        perror("setsockopt()");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"0.0.0.0",&server_addr.sin_addr);
        //监听套接字
    bind(serve_sk,(void*)&server_addr,sizeof(server_addr));
    

    if(listen(serve_sk,1024)<0)
    {
        perror("listen");
        exit(1);
    }
    printf("已经启动监听\n");
//用一个套接字来监听,等监听到请求 再创建新的套接字用于接收
    pid_t pid;
    for(int i = 0;i<PROCNUM;i++)
    {   
        
        pid = fork();
        if(pid<0)
        {
            perror("fork()");
            exit(1);
        }    
        else if(pid == 0)
        {   
            printf("子进程%d\n",i);
            server_loop(serve_sk);
            exit(0);
        }
    }
    for(int i = 0;i<PROCNUM;i++)
    {
        wait(NULL);
    }
}


