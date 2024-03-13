#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include "sem/mysem.h"
#define MAX 5
#define LEFT 30000001
#define RIGHT 30000200
static pthread_t pthl[RIGHT-LEFT+1];
struct sem* sem_st;
static void * func(void* p)
{
    int *n =p;
    int flag=1;
    for(int i=2;i<*n/2;i++)
    {
        if(*n%i==0)
        {
            flag=0;
            break;
        }    
    }
    if(flag)
    {
        printf("%d is primer\n",*n);
    }
    
    sleep(1);
    sem_add(sem_st,1);
    pthread_exit(NULL);
}
int main()
{   
    sem_st = sem_init(MAX);
    for(int i=LEFT,k=0;i<=RIGHT;i++,k++)
    {   
        sem_sub(sem_st,1);
        int *num = malloc(sizeof(int));
        *num = i;
        int err = pthread_create(pthl+k,NULL,&func,num);
    }

    for(int i=LEFT;i<=RIGHT;i++)
    {
        pthread_join(pthl[i-LEFT],NULL);
    }
    sem_destory(sem_st);
    exit(0);
}