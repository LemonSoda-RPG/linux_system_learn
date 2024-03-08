#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <signal.h>
static void int_handler(int s)
{
    write(1,"&&",2);
}
int main()
{   
    // signal(SIGINT,SIG_IGN);  // 忽略指定信号

    signal(SIGINT,int_handler);

    for(int i=0;i<10;i++)
    {
        write(1,"*****\n",6);
        sleep(1);
    } 

    exit(0);

}