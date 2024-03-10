#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/syscall.h>      /* Definition of SYS_* constants */
// #include <unistd.h>
static void int_handler(int s)
{
    write(1,"&&",2);
}
int main()
{   
    // signal(SIGINT,SIG_IGN);  // 忽略指定信号
    sigset_t set,saveset,oset;
    signal(SIGINT,int_handler);
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    //首先我需要一个unblock状态  因为我不知道之前是什么状态，那么我需要自己手动设置，并保存原本的状态
    sigprocmask(SIG_UNBLOCK,&set,&saveset);
    //之后我需要将信号进行block，所以要保存这个unblock的状态
    sigprocmask(SIG_BLOCK,&set,&oset);
    // sigprocmask(SIG_BLOCK, &set,&oset);
    for(int j=0;j<10;j++)
    {   
        for(int i=0;i<5;i++)
        {
            write(1,"*",1);
            sleep(1);
        } 
        putc('\n',stdout);
        // sigprocmask(SIG_UNBLOCK,&set,NULL);
        // sigprocmask(SIG_UNBLOCK,&oset,NULL);
        sigsuspend(&oset);  //这里是临时处理成oset 在响应结束之后，又会恢复成原状态
    }
    sigprocmask(SIG_SETMASK,&saveset,NULL);
    exit(0);
}