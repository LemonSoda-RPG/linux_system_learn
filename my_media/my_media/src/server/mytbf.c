#include<stdio.h>
#include<stdlib.h>
#include <syslog.h>
#include<pthread.h>
#include <unistd.h>
#include"mytbf.h"
// 多线程并发版
struct mytbf_t
{
    int cps;        // 每一秒的增量
    int burst;      // 上限
    int token;      // 当前的数量
    int pos;        // 这个令牌的下标  因为我们为每一个频道都设置了一个令牌 所以是一个数组
    pthread_mutex_t mut;
    pthread_cond_t cond;

};
static struct mytbf_t* job[MYTBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;
static pthread_t tid;
static int min(int a, int b) { return a < b ? a : b; }
static void *thr_alrm(void*p)
{
    while(1)
    {
        pthread_mutex_lock(&mut_job);
        // 每一秒 遍历所有进程的令牌  并加token  如果总大小大于burst了  就设置当前token为burst
        for(int i=0;i<MYTBF_MAX;i++)
        {
            if(job[i]!=NULL)
            {
                pthread_mutex_lock(&job[i]->mut);
                job[i]->token+=job[i]->cps;
                if(job[i]->token>job[i]->burst)
                    job[i]->token = job[i]->burst;
                pthread_cond_broadcast(&job[i]->cond);
                pthread_mutex_unlock(&job[i]->mut);

            }
        }
        pthread_mutex_unlock(&mut_job);
        sleep(1);
    }
    return NULL;
}

static int get_free_pos_unlocked()
{
    for(int i=0;i<MYTBF_MAX;i++)
    {
        if(job[i]==NULL)
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
        free(job[i]);
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

// 这个函数应该数在循环中被多次调用的  用于给每一个频道配置令牌
mytbf_t* mytbf_init(int cps,int burst)
{
    struct mytbf_t *me;
    
    pthread_once(&init_once,module_load);
    
    me = malloc(sizeof(*me));
    if(me==NULL)
        return NULL;
    me->burst = burst;
    me->cps = cps;
    me->token = 0;
    pthread_mutex_init(&me->mut,NULL);
    pthread_cond_init(&me->cond,NULL);
    pthread_mutex_lock(&mut_job);

     // 这里是对job数组进行加锁
    int pos = get_free_pos_unlocked(); 
    if(pos<0)
    {
        syslog(LOG_WARNING,"get_free_pos_unlocked");
        pthread_mutex_unlock(&mut_job);
        free(me);
        return NULL;
    }
    me->pos = pos;
    job[me->pos] = me;
    pthread_mutex_unlock(&mut_job);
    return me;
}
int mytbf_fetchtoken(mytbf_t* ptr,int size)  //取token
{
    int n;
    struct mytbf_t *me = ptr;
    pthread_mutex_lock(&me->mut);
    while (me->token<=0)
    {
        // 条件机制  解锁  把锁让给添加令牌的进程
        pthread_cond_wait(&me->cond,&me->mut);
    }
    n = min(me->token,size);
    printf("之后数量数量为%d\n",me->token);
    me->token-=n;
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);
    return n;
    
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
    job[me->pos] = NULL;
    pthread_mutex_unlock(&mut_job);
    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(ptr);
    return 0;
}

// 查看剩余令牌数量
int mytbf_checktoken(mytbf_t *ptr) {
    int token_left = 0;
    struct mytbf_t *me = ptr;
    pthread_mutex_lock(&me->mut);
    token_left = me->token;
    pthread_mutex_unlock(&me->mut);
    return token_left;
}