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
    int count_reader;  //读者数量
    int count_writer;  // 写者数量
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
    me->tail = 0;
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



    // 循环数组

}

int mypipe_register(mypipe_st* ptr,int opmap)
{

    struct mypipe_st * me=ptr; 
    pthread_mutex_lock(&me->mut);

    if(MYPIPE_READ&opmap)
    {
        me->count_reader++;
    }
    if(MYPIPE_WRITER&opmap)
    {
        me->count_writer++;
    }
    pthread_cond_broadcast(&me->con);  // 这里提醒的并不一定是当前线程  还有别的线程卡在下面这个while
    while(me->count_reader<=0 || me->count_writer<=0)  //要有一个时刻提醒 定义哪个时刻
    {
        pthread_cond_wait(&me->con,&me->mut);
    }
    pthread_mutex_unlock(&me->mut);

}

int mypipe_unregister(mypipe_st* ptr,int opmap)
{
    struct mypipe_st * me=ptr; 
    
    pthread_mutex_lock(&me->mut);

    if(MYPIPE_READ&opmap)
    {
        me->count_reader--;
    }
    if(MYPIPE_WRITER&opmap)
    {   
        me->count_writer--;
    }
    pthread_cond_broadcast(&me->con);   //告诉读的线程  有可能写者已经没了  

    pthread_mutex_unlock(&me->mut);
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
    // 如果没有数据  但是有写者，说明写者的数据还没有写入，我们要等待他写完再进行读取
    // 只有当有数据 或者没有写者之后 我们才会进行读取数据的操作
    while(me->datasize<=0 && me->count_writer>0)  
    {
        pthread_cond_wait(&me->con,&me->mut); //在等待期间会释放锁
       
    }
    // 上述锁有两种可能被打断 有可能有数据了 也可能是没有写者了   没有写者没有影响 但是要判断是否还有数据
    if(me->datasize<=0)
    {
        pthread_mutex_unlock(&me->mut);
        return 0;
    }
    

    for(i=0;i<size;i++)
    {
        if(mypipe_readbyte(me,buf+i)!=0)
            break;
    }
    pthread_cond_broadcast(&me->con);
    pthread_mutex_unlock(&me->mut);
    return i;
}
int mypipe_write(mypipe_st* st,void *buf,size_t size)
{
    
    struct mypipe_st * me = st;

}
