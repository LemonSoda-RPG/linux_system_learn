#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#define THNUM 5
#define LEFT 300000500
#define RIGHT 300008000
static pthread_t pthl[THNUM];
static int number;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t con_num = PTHREAD_COND_INITIALIZER; 

static void * func(void* p)
{
    while(1)
    {
        int flag=1,number2=0;
        pthread_mutex_lock(&mut_num);
        while(number==0)
        {
            pthread_cond_wait(&con_num,&mut_num);
        }
        if(number==-1)
        {
            pthread_mutex_unlock(&mut_num);
            break;
        }
        number2=number;
        number=0;
        pthread_cond_broadcast(&con_num);

        pthread_mutex_unlock(&mut_num);
        for(int i=2;i<number2/2;i++)
        {
            if(number2%i==0)
            {
                flag=0;
                break;
            }    
        }
        if(flag)
        {
            printf("%d is primer\n",number2);
        }
        
    }
    pthread_exit(NULL);
}
int main()
{

    for(int i=0;i<THNUM;i++)
    {
        int err = pthread_create(pthl+i,NULL,&func,NULL);
        if(err)
        {
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }


    for(int i=LEFT;i<=RIGHT;i++)
    {
        pthread_mutex_lock(&mut_num);
        while(number!=0)   //如果number！=0  说明number还没有被取走  我们要解锁 看有没有人能抢走
        {
            pthread_cond_wait(&con_num,&mut_num);
        }
        number = i;


        pthread_cond_broadcast(&con_num);
        // printf("%d\n",number);
        pthread_mutex_unlock(&mut_num);
    }



    pthread_mutex_lock(&mut_num);
    while(number!=0)   //如果number！=0  说明number还没有被取走  我们要解锁 看有没有人能抢走
    {
        
        pthread_cond_wait(&con_num,&mut_num);
    }
    number = -1;
    pthread_cond_broadcast(&con_num);
    pthread_mutex_unlock(&mut_num);
    

    for(int i=0;i<THNUM;i++)
    {
        pthread_join(pthl[i],NULL);
    }
    pthread_mutex_destroy(&mut_num);
    pthread_cond_destroy(&con_num);

    exit(0);
}