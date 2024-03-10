#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include "mytbf.h"
#define TBFSIZE 1024
// typedef void (*sighandler_t)(int);
static int inited = 0;
static struct mytbf_st* tbflist[TBFSIZE];
// static sighandler_t sig;
static struct sigaction sigsa;
static struct sigaction sigsasave;
static struct itimerval sigtm;

static void alrm_action(int s,siginfo_t *infop,void *unused)
{
    // alarm(1);

    if(infop->si_code!=SI_KERNEL)
        return;
    for(int i=0;i<TBFSIZE;i++)
    {
        if(tbflist[i]!=NULL)
        {
            tbflist[i]->token+=tbflist[i]->cps;
            tbflist[i]->token = tbflist[i]->token > tbflist[i]->burst ? tbflist[i]->burst : tbflist[i]->token;
        }
    }
}

static void module_unload(void)
{
    int i;
    // signal(SIGALRM,sig);
    // alarm(0);
    // sa版本
    sigaction(SIGALRM,&sigsasave,NULL);

    sigtm.it_value.tv_sec = 0;
    sigtm.it_value.tv_usec = 0;
    sigtm.it_interval.tv_sec = 0;
    sigtm.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL,&sigtm,NULL);


    for(i=0;i<TBFSIZE;i++)
    {
        free(tbflist[i]);
    }
}
static void module_load(void)
{

    // sig = signal(SIGALRM,alrm_handler);
    // alarm(1);
    // sa版本
   
    sigsa.sa_sigaction = alrm_action;
    sigemptyset(&sigsa.sa_mask);
    sigsa.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM,&sigsa,&sigsasave);
    // if error
    sigtm.it_value.tv_sec = 1;
    sigtm.it_value.tv_usec = 0;
    sigtm.it_interval.tv_sec = 1;
    sigtm.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL,&sigtm,NULL);
    // if error


    atexit(module_unload);
}

static int inserttbf(void)
{
    for(int i=0;i<TBFSIZE;i++)
    {
        if(tbflist[i]==NULL)
            return i;
    }
    return -1;
}
mytbf_st *mytbf_init(int cps,int burst)
{
    struct mytbf_st *st1;
    st1 = malloc(sizeof(*st1));
    if(!inited)
    {
        module_load();
        inited=1;
    }
    if(st1==NULL)
        return NULL;
    st1->cps=cps;
    st1->burst=burst;
    st1->token = 0;
    int i=inserttbf();
    if(i<0)
        return NULL;
    tbflist[i]=st1;
    st1->index=i;
    return st1;
}
int mytbf_fetchtoken(mytbf_st *s,int n)  //返回值是实际取走了几个token
{   
    struct mytbf_st*st =s;

    int i=0;
    if(n<=0)
        return 0; 
    while(st->token<=0)
        pause();
    if(n>=st->token)
    {
        i = st->token;
        st->token=0;
        return i;    
    }
    i=st->token-n;
    st->token=i;
    return n;
}

int mytbf_returntoken(mytbf_st *s,int n)
{   
    struct mytbf_st*st =s;
    if(n<=0)
    {
        return 0;
    }
    st->token += n;
    if(st->token>st->burst)
        st->token=st->burst;
    return n;
}

int mytbf_destory(mytbf_st *s)
{   

    struct mytbf_st*st =s;
    int index = st->index;
    free(st);
    tbflist[index]=NULL;
}