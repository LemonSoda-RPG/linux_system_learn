/*
    多播发送方  需要由发送方定义多播组
    接收方 先加入多播组 之后在进行接收消息
*/

#include <stdlib.h>
#include <net/if.h>
#include <stdio.h>
#include <stdint.h>
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
    if(strlen(argv[1])>NAMEMAX)
    {
        fprintf(stderr,"name is too long");
        exit(1);
    }
    struct sockaddr_in sockre;
    struct msg_st *sbufp;

    int sd;
    sd = socket(PF_INET,SOCK_DGRAM,0);
    
    if(sd<0){
        perror("错误1");
        exit(1);
    }

    struct ip_mreqn mreq;
    inet_pton(AF_INET,MTROUP,&mreq.imr_multiaddr);
    inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("eth0");
   
    //创建多播组 

    
    if(setsockopt(sd,IPPROTO_IP,IP_MULTICAST_IF,&mreq,sizeof(mreq))<0)
    {
        perror("setsockopt");
        exit(1);
    }
    
    
    
    size_t size = sizeof(struct msg_st)+strlen(argv[1]);
    sbufp = malloc(size);
  
    if(sbufp==NULL)
    {
        perror("malloc");
        exit(1);
    }
    strcpy(sbufp->name,argv[1]);
    sbufp->math = htonl(rand()%100);
    sbufp->chinese = htonl(rand()%100);
    sockre.sin_family = AF_INET;
    sockre.sin_port = htons(atoi(RCVPORT));
    

    inet_pton(AF_INET,MTROUP,&sockre.sin_addr);
    if(sendto(sd,sbufp,size,0,(void*)&sockre,sizeof(sockre))<0)
    {
        perror("错误2");
        exit(1);
    }

    close(sd);

    exit(0);
}