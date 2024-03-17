#include "relay.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#define BUFSIZE 900
static struct  relay_job_st*  rel_job[JOB_MAX];
static pthread_once_t init_once = PTHREAD_ONCE_INIT;
static pthread_mutex_t mut_joblist = PTHREAD_MUTEX_INITIALIZER;
struct relay_fsm_st{
    int state;
    int sfd;
    int dfd;
    int len;
    int pos;
    char buf[BUFSIZE];
    char *errstr;
    int64_t count;
};
struct relay_job_st
{
    /* data */
    int jobstate;
    int fd1;
    int fd2;
    int savefd1;
    int savefd2;
    int64_t count12;
    int64_t count21;
    struct relay_fsm_st  fsm12,fsm21;
};


static void fsm_driver(struct relay_fsm_st *fsm) {
    int ret;
    // 根据当前状态，决定下一步动作
    switch(fsm->state) {
        // 当前状态为读状态
        case STATE_R:
            fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
            if(fsm->len == 0) { // 读完文件
                fsm->state = STATE_T;
            }else if(fsm->len < 0) {
                if(errno == EAGAIN) { // 数据没有准备好
                    fsm->state = STATE_R;
                } else { // 真错
                    fsm->errstr = "read()";
                    fsm->state = STATE_Ex;
                }
            }else { // 转换为写状态
                fsm->pos = 0;
                fsm->state = STATE_W;
            }
            break;
        // 当前状态为写状态
        case STATE_W:
            ret = write(fsm->dfd, fsm->buf + fsm->pos, fsm->len);
            if(ret < 0) {
                if(errno == EAGAIN) {
                    fsm->state = STATE_W;
                } else {
                    fsm->errstr = "write()";
                    fsm->state = STATE_Ex;
                }
            }else {
                fsm->pos += ret;
                fsm->len -= ret;
                if(fsm->len == 0) { // 写够len个字节
                    fsm->state = STATE_R;
                } else { // 没有写够len个字节
                    fsm->state = STATE_W;
                }
            }
            break;
        case STATE_Ex:
            perror(fsm->errstr);
            fsm->state = STATE_T;
            break;
        case STATE_T:
            break;
        default:
            abort();
            break;
    }
}
static void * func(void *p)
{   
    while(1)
    {    
        pthread_mutex_lock(&mut_joblist);
        for(int i=0;i<JOB_MAX;i++)
        {
            if(rel_job[i]!=NULL)
            {
                if(rel_job[i]->jobstate==STATE_RUN)
                {   

                    fsm_driver(&rel_job[i]->fsm12);
                    fsm_driver(&rel_job[i]->fsm21);

                    if(rel_job[i]->fsm12.state == STATE_T && \
                            rel_job[i]->fsm21.state == STATE_T)
                    {
                        rel_job[i]->jobstate=STATE_OVER;  
                    }
                    
                }
            }
        }
        pthread_mutex_unlock(&mut_joblist);
    }
}
static void module_load(void)
{
    pthread_t pth;
    int err = pthread_create(&pth,NULL,&func,NULL);
    if(err)
    {
        fprintf(stderr,"pthread_create():%s\n",strerror(err));
        exit(1);
    }
}
static int get_free_pos(void)
{
    for(int i=0;i<JOB_MAX;i++)
    {
        if(rel_job[i]==NULL)
            return i;
    }
    return -1;
}

int add_job(int fd1,int fd2)
{
    struct relay_job_st *job;

    pthread_once(&init_once,&module_load);


    job = malloc(sizeof(*job));
    if(job==NULL)
        return -ENOMEM;

    job->fd1 = fd1;
    job->fd2 = fd2;
    job->jobstate = STATE_RUN;

    job->savefd1 = fcntl(fd1,F_GETFL);
    job->savefd2 = fcntl(fd2,F_GETFL);
    fcntl(fd1,F_SETFL,job->savefd1|O_NONBLOCK);
    fcntl(fd2,F_SETFL,job->savefd2|O_NONBLOCK);
    

    job->fsm12.sfd = fd1;
    job->fsm12.dfd = fd2;
    job->fsm12.state = STATE_R;
    
    
    job->fsm21.sfd = fd2;
    job->fsm21.dfd = fd1;
    job->fsm21.state = STATE_R;

    pthread_mutex_lock(&mut_joblist);


    int pos =get_free_pos();
    if(pos<0)
    {
        pthread_mutex_unlock(&mut_joblist);
        fcntl(fd1,F_SETFL,job->savefd1);
        fcntl(fd2,F_SETFL,job->savefd2);
        if(job!=NULL)
        {
            free(job);
            job = NULL;
        }
        return -ENOSPC;
    }
    rel_job[pos] = job;
    pthread_mutex_unlock(&mut_joblist);
    return pos;
    // 对于查找空位  要使用互斥锁  因为我这里在查找空位 但是另一个任务可能会删除任务或是添加任务
}   

int cancel_job(int id)
{



}



int wait_job(int id, struct relay_st* st);
int stat_job(int id, struct relay_st* st);
