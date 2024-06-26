#include<stdlib.h>
#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#define MAX 5
#define LEFT 30000001
#define RIGHT 30000200
static pthread_t pthl[RIGHT-LEFT+1];
static sem_t sem;
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
    sem_post(&sem);
    pthread_exit(NULL);
}
int main()
{   
    int err = sem_init(&sem,0,8);
    for(int i=LEFT,k=0;i<=RIGHT;i++,k++)
    {   
        sem_wait(&sem);
        int *num = malloc(sizeof(int));
        *num = i;
        int err = pthread_create(pthl+k,NULL,&func,num);
    }

    for(int i=LEFT;i<=RIGHT;i++)
    {
        pthread_join(pthl[i-LEFT],NULL);
    }
    sem_destroy(&sem);
    exit(0);
}