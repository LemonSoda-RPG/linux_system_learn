#include <stdio.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#define BUFERSIZE 1024
int main(int argc,char **argv)
{
    char rbuf[BUFERSIZE];
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
    remoteaddr.sin_port = htons(80);
    inet_pton(AF_INET,argv[1],&remoteaddr.sin_addr);
 
    if(connect(remote_sd,(void*)&remoteaddr,sizeof(remoteaddr))==-1)
    {
        perror("connect error:");
        exit(1);
    }

    FILE *fp;
    fp = fdopen(remote_sd,"r+");
   
    if(fp==NULL)
    {
        perror("fdopen");
        exit(1);
    }
    // printf("3\n");
 
    fprintf(fp,"GET /picture.png\r\n\r\n");
    fflush(fp);

    while(1)
    {
        int len = fread(rbuf,1,BUFERSIZE,fp);
        if(len==0)
            break;
        fwrite(rbuf,1,len,stdout);
    }


    fclose(fp);
    exit(0);

}