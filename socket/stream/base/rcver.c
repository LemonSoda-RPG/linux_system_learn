#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "proto.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
  #include <arpa/inet.h>
// #include <sys/socket.h>
#include <unistd.h>
int main()
{   
    size_t size;
    size = sizeof(struct msg_st) + NAMEMAX - 1; 
    struct sockaddr_in sockin,sockre;
    struct msg_st *rbufp;
    rbufp = malloc(size);
    memset(&sockin,0,sizeof(sockin));
    memset(&sockre,0,sizeof(sockre));
    sockin.sin_family = AF_INET;
    sockin.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET,"0.0.0.0",&sockin.sin_addr);
    
    int sd;
    sd = socket(PF_INET,SOCK_DGRAM,0);

    if(bind(sd,(void*)&sockin,sizeof(sockin))<0)
    {
        perror("");
        exit(1);
    }
    int val =1;
    if(setsockopt(sd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val))<0)
    {
        perror("");
        exit(1);
    }

    socklen_t sockrelen = sizeof(sockin);
    while(1)
    {
        recvfrom(sd,rbufp,size,0,(void*)&sockre,&sockrelen);
        printf("接收成功");
        printf("name = %s\n",rbufp->name);
        printf("math = %d\n",ntohl(rbufp->math));
        printf("chinese = %d\n",ntohl(rbufp->chinese));
    }
    close(sd);

    exit(0);

}