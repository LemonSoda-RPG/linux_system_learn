#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include "anytimer.h"

static struct at_job_st* joblist[JOB_MAX];
static int bool = 1;
enum 
{
    STATE_RUNNING = 1,
    STATE_CANCELED,
    STATE_OVER
};
struct at_job_st
{
    int job_state;
    int sec;
    int time_remain;
    // int repeat;   // 1真  0假
    at_jobfunc_t *jobptr;
    void *arg;
};

static void alrm_handler(int s)
{
    alarm(1);
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
static void module_load(void)
{

    // struct sigaction sa, old_sa;
    
    if(sigaction(SIGALRM,&sa)<0)
    {
        perror("sigaction");
    }
    alarm(1);
    setitimer();
    atexit(module_unload);
}

static void module_unload(void)
{
    sigaction();
    setitimer();

}
int at_addjob(int sec, at_jobfunc_t *jobptr,void *arg)
{
    struct at_job_st *stptr;
    int index;
    

    if(bool)
    {   

        module_load();
        bool = 0;
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
    stptr->arg = arg;
    joblist[index] = stptr;
    return index;

}

int at_canceljob(int id)
{
    if(id<0||id>=JOB_MAX||joblist[id]==NULL)
    {
        return -EINVAL;
    }
    if(joblist[id]->job_state==STATE_CANCELED)
        return -ECANCELED;
    if(joblist[id]->job_state==STATE_OVER)
        return -EBUSY;
    joblist[id]->job_state = STATE_CANCELED;
    return 0;
}

// int at_canceljob(int id);
int at_waitjob(int id)
{
    if(id < 0 || id >= JOB_MAX)
    {
        return -EINVAL;
    }
    while(joblist[id]->job_state==STATE_RUNNING)
        pause();
    if(joblist[id]->job_state==STATE_CANCELED||joblist[id]->job_state==STATE_OVER)
    {
        free(joblist[id]);
        joblist[id]=NULL;
    }
    return 0;
}

