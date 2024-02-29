#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
 #include <fcntl.h>
#include <sys/types.h>
#define FNAME "/tmp/out"
static int daemonize(void)
{
    pid_t pid;
    int fd;
    pid=fork();
    if(pid<0)
    {
        perror("fork()");
        return -1;
    }
   
    if(pid>0)
    {
        exit(0);
    }
    // 为了摆脱tty  重定向0 1 2
    fd = open("/dev/null",O_RDWR);
    printf("文件描述符为%d\n",fd);
    if(fd<0)
    {
        perror("open()");
        return -1;
    }
    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);
    
    if(fd>2)
    {
        close(fd);
    }
    setsid();
    chdir("/");
    return 0;
    
}


int main()
{   
    FILE *fp;
    if(daemonize())
    {
        exit(1);
    }
    fp=fopen(FNAME,"w");
    if(fp==NULL)
    {
        perror("fopen()");
        exit(1);
    }
    while(1)
    {
        fprintf(fp,"1\n");
        fflush(fp);
        sleep(1);
    }
    exit(0);
}