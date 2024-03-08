#ifndef ANYTIMER_H__
#define ANYTIMER_H__

#define JOB_MAX 1024 
typedef void (at_jobfunc_t)(void*);
static struct at_job_st* joblist[JOB_MAX];

int at_addjob(int sec, at_jobfunc_t *,void *arg);
/*
    return  >= 0;
            == 0



*/
int at_canceljob(int id);
int at_waitjob(int id);
#endif