#include "mysem.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
typedef void sem;
struct sem
{
    int threadnum;
    pthread_cond_t con;
    pthread_mutex_t mut;
};
void *sem_init(int n)
{
    struct sem* sem_st;
    sem_st = malloc(sizeof(*sem_st));
    sem_st->threadnum = n;
    pthread_cond_init(&sem_st->con,NULL);
    pthread_mutex_init(&sem_st->mut,NULL);
    return sem_st;
}
void sem_add(void *st,int n)
{
    struct sem* sem_st = st;
    pthread_mutex_lock(&sem_st->mut);
    sem_st->threadnum += n;
    pthread_cond_broadcast(&sem_st->con);
    pthread_mutex_unlock(&sem_st->mut);
}
void sem_sub(void *st,int n)
{
    struct sem* sem_st = st;
    pthread_mutex_lock(&sem_st->mut);
    while(n>sem_st->threadnum)
        pthread_cond_wait(&sem_st->con,&sem_st->mut);
    sem_st->threadnum -= n;
    pthread_mutex_unlock(&sem_st->mut);
}

void sem_destory(void *st)
{
    struct sem* sem_st = st;
    pthread_cond_destroy(&sem_st->con);
    pthread_mutex_destroy(&sem_st->mut);
    free(st);

}





