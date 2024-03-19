#include <stdlib.h>
#include <stdio.h>
// #include <pthread.h>
#include <pthread.h>
#include "mypipe.h"
struct mypipe_st
{
    /* data */
    int head;
    int tail;
    char data[PIPESIZE];
    int datasize;
    pthread_mutex_t mut;
    pthread_cond_t con;

};

mypipe_st *mypipe_init(void)
{
    struct mypipe_st *me;
    me = malloc(sizeof(*me));
    if(me==NULL)
        return NULL; // 尽量不要在库里面进行报错
    me->head = 0;
    me->tail = -1;
    me->datasize = 0;
    pthread_mutex_init(&me->mut,NULL);
    pthread_cond_init(&me->con,NULL);
    return me;
}

int mypipe_destroy(mypipe_st* st)
{
    struct mypipe_st * me = st;
    pthread_mutex_destroy(&me->mut);
    pthread_con_destroy(&me->con);
    free(st);
    return 0;
}
int next(int)
{

}

int  mypipe_readbyte(struct mypipe_st *me,char *datap)
{
    if(me->datasize<=0)
    {
        return -1;
    }
    *datap = me->data[me->head];
    me->datasize--;
    me->head = next(me->head);
    return 0;  
}
int mypipe_read(mypipe_st* st,void *buf,size_t size)
{
    int i;
    struct mypipe_st * me = st;
    pthread_mutex_lock(&me->mut);
    while(me->datasize<=0)
    {
        pthread_cond_wait(&me->con,&me->mut); //在等待期间会释放锁
    }
    for(i=0;i<size;i++)
    {
        if(mypipe_readbyte(me,buf+i)!=0)
            break;
    }
    pthread_mutex_unlock(&me->mut);
    return i;
}
int mypipe_write(mypipe_st* st,void *buf,size_t size)
{
    
    struct mypipe_st * me = st;

}
