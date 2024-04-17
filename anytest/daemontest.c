#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
int main()
{
    
    pid_t pid = fork();
    if(pid>0)
        exit(0);
    if(pid==0)
    {   setsid();

        FILE *fp;
        fp = fopen("/tmp/out","w+");
        int i = 0;
        while(1)
        {
            fprintf(fp,"%d",i++);
            fflush(fp);
            sleep(1);
        }
        exit(1);
    }
}

