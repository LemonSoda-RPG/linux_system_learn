#include<stdio.h>
#include<stdlib.h>
#include <syslog.h>
#include<pthread.h>
#include <unistd.h>
#include"mytbf.h"
struct mytbf_t
{
    int cps;
    int burst;
    int token;
    int pos;
    pthread_mutex_t mut;
    pthread_cond_t cond;

};
static struct mytbf_t* tbf_list[MYTBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;
static pthread_t tid;
static void *thr_alrm(void*p)
{
    while(1)
    {
        pthread_mutex_lock(&mut_job);
        for(int i=0;i<MYTBF_MAX;i++)
        {
            if(tbf_list[i]!=NULL)
            {
                pthread_mutex_lock(&tbf_list[i]->mut);
                tbf_list[i]->token+=tbf_list[i]->cps;
                if(tbf_list[i]->token>tbf_list[i]->burst)
                    tbf_list[i]->token = tbf_list[i]->burst;
                pthread_cond_broadcast(&tbf_list[i]->cond);
                pthread_mutex_unlock(&tbf_list[i]->mut);

            }
        }
        pthread_mutex_unlock(&mut_job);
        sleep(1);
    }
}
static int get_free_pos_unlocked()
{
    for(int i=0;i<MYTBF_MAX;i++)
    {
        if(tbf_list[i]==NULL)
            return i;
    }
    return -1;
}
static void module_unload(void)
{
    pthread_cancel(tid);
    pthread_join(tid,NULL);
    for(int i =0;i<MYTBF_MAX;i++)
    {
        free(tbf_list[i]);
    }
}
static void module_load(void)
{
    int err;
   
    err = pthread_create(&tid,NULL,thr_alrm,NULL);
    if(err)
    {
        fprintf(stderr,"pthread_create():");
        exit(1);
    }
    atexit(module_unload);
}
mytbf_t* mytbf_init(int cps,int burst)
{
    struct mytbf_t *me;

    pthread_once(&init_once,module_load);

    me = malloc(sizeof(struct mytbf_t));
    if(me==NULL)
        return NULL;
    me->burst = burst;
    me->cps = cps;
    me->token = 0;
    pthread_mutex_init(&me->mut,NULL);
    pthread_cond_init(&me->cond,NULL);
    pthread_mutex_lock(&mut_job);
    int index = get_free_pos_unlocked(); 
    if(index<0)
    {
        syslog(LOG_WARNING,"get_free_pos_unlocked");
        pthread_mutex_unlock(&mut_job);
        free(me);
        return NULL;
    }
    tbf_list[index] = me;
    pthread_mutex_unlock(&mut_job);
    me->pos = index;
}
int mytbf_fetchtoken(mytbf_t* ptr,int size)  //取token
{
    // int n;
    struct mytbf_t *me = ptr;
    pthread_mutex_lock(&me->mut);
    while (me->token<=0)
    {
        pthread_cond_wait(&me->cond,&me->mut);
    }
    if(me->token<size)
        size = me->token;
    me->token -= size;
    pthread_mutex_unlock(&me->mut);
    return size;
    
}  
int mytbf_returntoken(mytbf_t*ptr,int size)  //还token
{
    struct mytbf_t *me = ptr;
    pthread_mutex_lock(&me->mut);
    me->token+=size;
    if(me->token>me->burst)
        me->token = me->burst;
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);
    return 0;
}
int mytbf_destory(mytbf_t* ptr)       //销毁
{
    struct mytbf_t* me = ptr;
    pthread_mutex_lock(&mut_job);
    tbf_list[me->pos] = NULL;
    pthread_mutex_unlock(&mut_job);
    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(ptr);
    return 0;
}