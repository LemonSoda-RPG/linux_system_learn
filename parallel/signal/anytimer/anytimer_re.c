/*
    不是多线程！！不是多线程  
    只是将信息预先存储好，每过1秒就遍历之前存储的信息，将他们的寿命减1，等到寿命为0的时候
    读取这个任务的信息，并进行输出
*/


#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include "anytimer.h"

static struct itimerval itv;
static int flag = 1;
static struct sigaction osa;
enum 
{
    STATE_RUNNING = 1,
    STATE_CANCELED,
    STATE_OVER
};

struct at_job_st
{
    //多久之后  执行某个任务
    int job_state;
    int sec;
    int time_remain;  // 任务寿命
    int repeat;   // 1真  0假  //是否是周期性的任务
    at_jobfunc_t *jobptr;
    void *arg;
};

static struct at_job_st* joblist[JOB_MAX];  //里面存储的是指针

static void alarm_action(int s, siginfo_t *infop, void *unused)
{
    if(infop->si_code!=SI_KERNEL)
        return;
    for(int i=0;i<JOB_MAX;i++)
    {
        if(joblist[i]!=NULL&&joblist[i]->job_state==STATE_RUNNING)
        {
            joblist[i]->time_remain--;
            if(joblist[i]->time_remain==0)
            {
                joblist[i]->jobptr(joblist[i]->arg);   //会有累计误差  而且如果函数执行的很久  那就无法计时了
                if(joblist[i]->repeat==1)
                {
                    joblist[i]->time_remain = joblist[i]->sec;
                }
                else
                    joblist[i]->job_state = STATE_OVER;
                
                
            }
        }
    }
}


static int get_free_pos(void)
{
    int i;
    for(i=0;i<JOB_MAX;i++)
    {
        if(joblist[i]==NULL)
        {
            return i;
        }
    }
    return -1;
}

static void module_unload(void)
{
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL,&itv,NULL);
    if(sigaction(SIGALRM,&osa,NULL)<0)
    {
        perror("sigaction");
        exit(1);
    }
}


static void module_load(void)
{

    // struct sigaction sa, old_sa;
    
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = alarm_action;
    
    if(sigaction(SIGALRM,&sa,&osa)<0)
    {
        perror("sigaction");
        exit(1);
    }
    // alarm(1);

    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    if(setitimer(ITIMER_REAL,&itv,NULL)<0)
    {
        perror("setitimer");
        exit(1);
    }
    atexit(module_unload);
}


int at_addjob(int sec, at_jobfunc_t *jobptr,void *arg)
{


    if(sec<0)
    {
        return -EINVAL;
    }
    struct at_job_st *stptr;
    int index;
    

    if(flag)
    {   
        module_load();
        flag = 0;
    }

    index = get_free_pos();
    if(index<0)
    {
        return -ENOSPC;
    }
    stptr = malloc(sizeof(*stptr));
    if(stptr==NULL)
    {
        return -ENOMEM;
    }
    stptr->job_state = STATE_RUNNING;
    stptr->sec = sec;
    stptr->time_remain = sec;
    stptr->jobptr = jobptr;
    stptr->arg = arg;  //给信号处理函数的传参

    stptr->repeat = 0;
    joblist[index] = stptr;  //将任务信息放到这个列表中
    return index;

}
int at_addjob_repeat(int sec,at_jobfunc_t *jobp,void *arg)
{
    if(sec<0)
    {
        return -EINVAL;
    }
    if(!flag)
    {
        module_load();
        flag =1;
    }
    int index = get_free_pos();
    if(index<0)
    {
        return -ENOSPC;
    }
    struct at_job_st *stptr;
    stptr = malloc(sizeof(*stptr));
    if(stptr==NULL)
    {
        return -ENOMEM;
    }

    stptr->job_state = STATE_RUNNING;
    stptr->sec = sec;
    stptr->time_remain = sec;
    stptr->jobptr = jobp;
    stptr->arg = arg;  //给信号处理函数的传参
    stptr->repeat = 1;  
    joblist[index] = stptr;  //将任务信息放到这个列表中
    return index;

}
int at_canceljob(int id)  //这里的id就是下标
{
    if(id<0||id>=JOB_MAX||joblist[id]==NULL)
    {
        return -EINVAL;
    }
    if(joblist[id]->job_state==STATE_CANCELED)  //任务已经被取消了   
        return -ECANCELED; 
    if(joblist[id]->job_state==STATE_OVER)  //说明任务已经结束了，完成并结束，而不是被取消了
        return -EBUSY;
    joblist[id]->job_state = STATE_CANCELED;
    return 0;
} //仅仅只是取消，不包含收尸的后续操作


int at_waitjob(int id)
{
    if(id < 0 || id >= JOB_MAX)
    {
        return -EINVAL;
    }

    if(joblist[id]->repeat==1)
    {
        return -EBUSY;
    }
    while(joblist[id]->job_state==STATE_RUNNING)     //如果满足条件，则函数在此终止，等待任务结束或是被打断
        pause();
    if(joblist[id]->job_state==STATE_CANCELED||joblist[id]->job_state==STATE_OVER)
    {
        free(joblist[id]);
        joblist[id]=NULL;
    }
    return 0;
}

