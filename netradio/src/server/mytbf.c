#include<stdio.h>
#include<stdlib.h>
#include <syslog.h>
#include<pthread.h>
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
static int get_free_pos_unlocked()
{
    for(int i=0;i<MYTBF_MAX;i++)
    {
        if(tbf_list[i]==NULL)
            return i;
    }
    return -1;
}
mytbf_t* mytbf_init(int cps,int burst)
{
    struct mytbf_t *me;
    me = malloc(sizeof(struct mytbf_t));
    if(me==NULL)
        reutrn NULL;
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
int mytbf_fetchtoken(mytbf_t*,int)  //取token
{

}  
int mytbf_returntoken(mytbf_t*,int)  //还token
{

}
int mytbf_destory(mytbf_t* p)       //销毁
{
    struct mytbf_t* me = p; 
    
}