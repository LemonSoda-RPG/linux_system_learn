#include <stdio.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <stdlib.h>
#include "proto.h"
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
int main(int argc,char **argv)
{
    long long stamp;
    if(argc<2)
    {
        fprintf(stderr,"参数不足");
        exit(1);
    }
 
    int remote_sd = socket(AF_INET,SOCK_STREAM,0);
    if(remote_sd<0)
    {
        perror("");
        exit(1);
    }
    struct sockaddr_in remoteaddr;
    remoteaddr.sin_family=AF_INET;
    remoteaddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,argv[1],&remoteaddr.sin_addr);
    // printf("asdfasdfasdfasd");
    if(connect(remote_sd,(void*)&remoteaddr,sizeof(remoteaddr))==-1)
    {
        perror("connect error:");
        exit(1);
    }
    // printf("1\n");
   
    FILE *fp;
    fp = fdopen(remote_sd,"r+");
    // printf("2\n");
   
    if(fp==NULL)
    {
        perror("fdopen");
        exit(1);
    }
    // printf("3\n");
  
    if(fscanf(fp,FMT_STAMP,&stamp)<1)
    {
        fprintf(stderr,"bad format!\n");
    }
    else
        fprintf(stdout,"stamp = %lld\n",stamp);
    
    fclose(fp);
    exit(0);

}