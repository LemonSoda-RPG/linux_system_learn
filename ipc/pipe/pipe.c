#include<stdio.h>
#include<stdlib.h>
#include<aio.h>
#include<unistd.h>
#include<wait.h>
#define BUFSIZE 1024
int main()
{
    int pd[2];  
    pid_t pid;
    if(pipe(pd)<0)
    {
        perror("pipe");
        exit(1);
    }

    close(pd[0]);
   
    write(pd[1],"hello!",6);
    wait(NULL);
    exit(0);
}