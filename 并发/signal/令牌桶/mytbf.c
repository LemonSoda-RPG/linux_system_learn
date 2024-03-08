#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "mytbf.h"
#define TBFSIZE 1024
typedef void (*sighandler_t)(int);
static int inited = 0;
static struct mytbf_st* tbflist[TBFSIZE];
static sighandler_t sig;
struct mytbf_st
{
    int cps;
    int token;
    int burst;
    int index;
};
static void alrm_handler(int s)
{
    alarm(1);
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
    signal(SIGALRM,sig);
    alarm(0);
    for(i=0;i<TBFSIZE;i++)
    {
        free(tbflist[i]);
    }
}
static void module_load(void)
{

    sig = signal(SIGALRM,alrm_handler);
    alarm(1);
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
struct mytbf_st *mytbf_init(int cps,int burst)
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
int mytbf_fetchtoken(struct mytbf_st *st,int n)  //返回值是实际取走了几个token
{
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

int mytbf_returntoken(struct mytbf_st *st,int n)
{
    if(n<=0)
    {
        return 0;
    }
    st->token += n;
    if(st->token>st->burst)
        st->token=st->burst;
    return n;
}

int mytbf_destory(struct mytbf_st *st)
{
    int index = st->index;
    free(st);
    tbflist[index]=NULL;
}