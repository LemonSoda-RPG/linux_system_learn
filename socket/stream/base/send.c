#include <stdlib.h>
#include <stdio.h>
#include "proto.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <unistd.h>
int main(int argc,char **argv)
{
    if(argc<2)
    {
        printf("参数不足\n");
        exit(1);
    }
    struct sockaddr_in sockre;
    struct msg_st sbuf;
    int sd;
    sd = socket(PF_INET,SOCK_DGRAM,0);
    if(sd<0){
        perror("错误1");
        exit(1);
    }
    strcpy(sbuf.name,"jiacheng");
    sbuf.math = htonl(rand()%100);
    sbuf.chinese = htonl(rand()%100);

    sockre.sin_family = AF_INET;
    sockre.sin_port = htons(atoi(RCVPORT));
    sockre.sin_addr.s_addr = inet_addr(argv[1]);

    if(sendto(sd,&sbuf,sizeof(sbuf),0,(void*)&sockre,sizeof(sockre))<0)
    {
        perror("错误2");
        exit(1);
    }

    close(sd);

    exit(0);
}