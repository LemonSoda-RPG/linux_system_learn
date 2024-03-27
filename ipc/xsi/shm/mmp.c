/*
内存映射
*/
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <aio.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <sys/mman.h>
#define MEMSIZE 1024
int main()
{
    char *ptr;
    pid_t pid;
    ptr =  mmap(NULL,MEMSIZE,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    
    if(ptr ==MAP_FAILED)
    {
        perror("");
    }
    pid = fork();
    if(pid<0)
    {
        perror("");
        munmap(ptr,MEMSIZE);
        exit(1);
    }
    if(pid==0)
    {
        strcpy(ptr,"hello");
    }
    else{
        wait(NULL);
        puts(ptr);
        munmap(ptr,MEMSIZE);
        exit(0);
    }
}