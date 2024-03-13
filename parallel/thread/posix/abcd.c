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
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t con = PTHREAD_COND_INITIALIZER;
static int number = 0;
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
    int *m =p;
    int n= *m;
    int c = 'a'+ n;
    while(1)
    {
        pthread_mutex_lock(&mut);
        while(n!=number)
        {
            pthread_cond_wait(&con,&mut);
            // pthread_mutex_lock(mut+n);
            // pthread_mutex_unlock(mut+next(n));
        }
        printf("%c",c);
        number = next(number);
        pthread_cond_broadcast(&con);
        pthread_mutex_unlock(&mut);

    }
    pthread_exit(NULL);
}


int main()
{
    pthread_t tid[THRNUM];
    for(int i=0;i<THRNUM;i++)
    {   
        // pthread_mutex_lock(mut+i);
        int * p =malloc(sizeof(int));
        *p = i;
        pthread_create(tid+i,NULL,&func,p);
    }
   
  
    alarm(1);

    for(int i=0;i<THRNUM;i++)
    {
        pthread_join(tid[i],NULL);
        pthread_cond_destroy(&con);
        pthread_mutex_destroy(&mut);
    }

    exit(0);


}