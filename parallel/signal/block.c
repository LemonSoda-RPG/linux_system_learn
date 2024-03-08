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
    sigset_t set;
    signal(SIGINT,int_handler);
    sigemptyset(&set);
    sigaddset(&set,SIGINT);

    for(int j=0;j<10;j++)
    {   
        sigprocmask(SIG_BLOCK,&set,NULL);
        for(int i=0;i<3;i++)
        {
            write(1,"*",1);
            sleep(1);
        } 
        putc('\n',stdout);
        sigprocmask(SIG_UNBLOCK,&set,NULL);
        
    }

    exit(0);

}