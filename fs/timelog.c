#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<time.h>
#include<unistd.h>
#define OUTNAME  "/tmp/out"
#define BUFFERSIZE 1024 
int main()
{
    FILE *fd;
    time_t tmres;
    struct tm *tm_res;
    fd = fopen(OUTNAME,"a+");
    if(fd == NULL)
    {
        perror("fopen");
        exit(1);
    }
    char buffer[BUFFERSIZE];
    int count=0;
    while(fgets(buffer,BUFFERSIZE,fd))
    {
        count++;
    }
    while(1)
    {
        time(&tmres);
        tm_res=localtime(&tmres);
        fprintf(fd,"%d %d-%d-%d %d-%d-%d\n",++count,\
                                tm_res->tm_year+1900,tm_res->tm_mon+1,tm_res->tm_mday,\
                                tm_res->tm_hour,tm_res->tm_min,tm_res->tm_sec);
        fflush(fd);
        sleep(1);
    }
    fclose(fd);
    exit(0);
}