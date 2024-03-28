#include <stdlib.h>
#include <stdio.h>
#include "proto.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
int main()
{   
    
    struct sockaddr_in sockin,sockre;
    struct msg_st rbuf;
    memset(&sockin,0,sizeof(sockin));
    memset(&sockre,0,sizeof(sockre));
    sockin.sin_family = AF_INET;
    sockin.sin_port = htons(atoi(RCVPORT));
    sockin.sin_addr.s_addr = htons(INADDR_ANY);
    int sd;
    sd = socket(PF_INET,SOCK_DGRAM,0);

    if(bind(sd,(void*)&sockin,sizeof(sockin))<0)
    {
        perror("");
        exit(1);
    }

    socklen_t sockrelen = sizeof(sockin);
    while(1)
    {
        recvfrom(sd,&rbuf,sizeof(rbuf),0,(void*)&sockre,&sockrelen);
        printf("接收成功");
        printf("name = %s\n",rbuf.name);
        printf("math = %d\n",ntohl(rbuf.math));
        printf("chinese = %d\n",ntohl(rbuf.chinese));
    }
    close(sd);

    exit(0);

}