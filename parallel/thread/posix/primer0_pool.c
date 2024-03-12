#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>
#include <string.h>
#define LEFT 300000500
#define RIGHT 300001000
#define THREADNUM 4
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_t pthl[THREADNUM];
static int number = 0;

//交叉算法计算 池类算法涉及到竞争
static void *func(void *p)
{   
    int i,j,mark;
    while(1)
    {
        pthread_mutex_lock(&mut_num);
        while(number==0)
        {
            pthread_mutex_unlock(&mut_num);
            sched_yield();
            pthread_mutex_lock(&mut_num);
        }
        if(number==-1)
        {   
            pthread_mutex_unlock(&mut_num);
            break;
        }
        i=number;
        number=0;
        pthread_mutex_unlock(&mut_num);
        mark = 1;
        for (j = 2;j <= i/2;j++)
        {
            if (i%j == 0)
            {
                mark = 0;
                break;
            }
        }
        if (mark) 
        {
            printf("%d is a primer\n",i);
        }
    }
    pthread_exit(NULL);
}
int main()
{
    // pthread_t pt;
    
    for(int i=0;i<THREADNUM;i++)
    {   
        
         // 分配内存给中间变量 
        int err = pthread_create(pthl+i,NULL,&func,NULL);
        if(err)
        {
            fprintf(stderr,"%s\n",strerror(err));
            for(int k=0;k<THREADNUM;k++)
            {
                if(pthl[k])
                {
                    int num = pthread_join(pthl[k],NULL);
                    if(num)
                    {
                        fprintf(stderr,"%s\n",strerror(num));
                        exit(1);
                    }
                }
            }
            exit(1);
        }
    }


    for(int i=LEFT;i<=RIGHT;i++)
    {
        pthread_mutex_lock(&mut_num);
        while(number!=0)
        {
            pthread_mutex_unlock(&mut_num);
            sched_yield();
            pthread_mutex_lock(&mut_num);
        }
        number = i;
        pthread_mutex_unlock(&mut_num);

    }

    pthread_mutex_lock(&mut_num);
    while(number!=0)
    {
        pthread_mutex_unlock(&mut_num);
        sched_yield();
        pthread_mutex_lock(&mut_num);
    }
    number = -1;
    pthread_mutex_unlock(&mut_num);


    for(int i=0;i<THREADNUM;i++)
    {
        if(pthl[i])
        {
            int num = pthread_join(pthl[i],NULL);   //ptr用于接收返回的值
            
            if(num)
            {
                fprintf(stderr,"%s\n",strerror(num));
                exit(1);
            }
        }
    }




    pthread_mutex_destroy(&mut_num);
    exit(0);
}
