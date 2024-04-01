#include <stdlib.h>
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
    if(argc<3)
    {
        printf("参数不足\n");
        exit(1);
    }
    if(strlen(argv[2])>NAMEMAX)
    {
        fprintf(stderr,"name is too long\n");
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

    int val = 1;
    if(setsockopt(sd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val))<0)
    {
        perror("setsockopt");
        exit(1);
    }
    
    size_t size = sizeof(struct msg_st)+strlen(argv[2]);
    sbufp = malloc(size);
  
    if(sbufp==NULL)
    {
        perror("malloc");
        exit(1);
    }
    strcpy(sbufp->name,argv[2]);
    sbufp->math = htonl(rand()%100);
    sbufp->chinese = htonl(rand()%100);
    sockre.sin_family = AF_INET;
    sockre.sin_port = htons(atoi(RCVPORT));
    // sockre.sin_addr.s_addr = inet_addr(argv[1]);
    // inet_pton(AF_INET,argv[1],&sockre.sin_addr);
    inet_pton(AF_INET,"255.255.255.255",&sockre.sin_addr);
    if(sendto(sd,sbufp,size,0,(void*)&sockre,sizeof(sockre))<0)
    {
        perror("错误2");
        exit(1);
    }

    close(sd);

    exit(0);
}