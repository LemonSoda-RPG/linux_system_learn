#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
// static pthread_t tidlist[102400];
void*func(void*p)
{
    int i;
    printf("%p\n",&i); 
    while(1)
        pause();
    pthread_exit(NULL);
}
int main()
{
    int i;
    pthread_t tid;   //tid只是用于接收返回的线程信息，创建线程之后 tid信息的改变对线程本身没有任何影响
    pthread_attr_t att;
    pthread_attr_init(&att);
    pthread_attr_setstacksize(&att,1024*1024*1024);
    for(i=1;;i++)
    {
        int err  = pthread_create(&tid,&att,&func,NULL);
        // int err  = pthread_create(&tid,NULL,&func,NULL);
        sleep(1);
        if(err)
        {
            break;
        }
    }
    printf("%d\n",i);
    pthread_attr_destroy(&att);
    exit(0);
}