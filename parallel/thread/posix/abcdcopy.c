/*
采用通知法
*/


#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include <unistd.h>
#define THRNUM 4
#define BUFFERSIZE 1024
#define FNAME "/tmp/out"
static pthread_mutex_t mut[THRNUM];
static pthread_cond_t con[THRNUM];
int next(int n)
{
    if((n+1)==THRNUM)
    {
        // printf("\n");
        return 0;

    }
    return n+1;
}
void *func(void* p)
{   
    int n=(int)p;
    int c = 'a'+ n;
    while(1)
    {
        pthread_cond_wait(con+n,mut+n);
        // pthread_mutex_lock(mut+n);
        printf("%c",c);
        pthread_cond_signal(con+next(n));
        // pthread_mutex_unlock(mut+next(n));
    }
    pthread_exit(NULL);
}


int main()
{
    pthread_t tid[THRNUM];
    for(int i=0;i<THRNUM;i++)
    {   
        pthread_mutex_init(mut+i,NULL);
        pthread_cond_init(con+i,NULL);
        // pthread_mutex_lock(mut+i);
        pthread_create(tid+i,NULL,&func,(void *)i);
    }
    pthread_cond_signal(con+0);
  
    alarm(1);

    for(int i=0;i<THRNUM;i++)
    {
        pthread_join(tid[i],NULL);
        pthread_cond_destroy(con+i);
        pthread_mutex_destroy(mut+i);
    }

    exit(0);


}  