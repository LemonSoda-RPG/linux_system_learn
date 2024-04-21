#include "thr_channel.h"
#include <stdlib.h>
#include <stdio.h>
#include "proto.h"
#include <pthread.h>
#include "site_type.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <syslog.h>
#include "server_conf.h"
static int tid_nextpos = 0;
struct thr_channel_ent_sdt
{
    chid_t chid;
    pthread_t tid;
};
struct thr_channel_ent_sdt  thr_channel[CHNNR];
static void *thr_channel_sender(void*p)
{
    struct mlib_listentry_st *ent = p;
    int datasize;
    int len;
    struct msg_channel_st *sbufp;
    sbufp = malloc(MSG_CHANNEL_MAX);
    if(sbufp==NULL)
    {
        syslog(LOG_ERR,"malloc failed():");
        exit(1);
    }
    // datasize = MAX_DATA
    sbufp->chid = ent->chid;
    while(1){
        len = mlib_readchn(ent->chid,sbufp->data,MAX_DATA);
        if(sendto(serversd,sbufp,len+sizeof(chid_t),0,(void*)&sndaddr,sizeof(sndaddr))<0)
        {
            syslog(LOG_ERR,"sendto(): %s",ent->chid);
        }
        sched_yield();
    }

    pthread_exit(NULL);
}
int thr_channel_create(struct mlib_listentry_st* ptr)
{
    int tid_nextpos = 0;
    int err;
    err = pthread_create(&thr_channel[tid_nextpos].tid,NULL,thr_channel_sender,ptr);
    if(err)
    {
        return -err;
    }
    thr_channel[tid_nextpos].chid = ptr->chid;

    tid_nextpos++;
    return 0;
}
int thr_channel_destory(struct mlib_listentry_st* ptr)
{
    for(int i =0;i<CHNNR;i++)
    {
        if(thr_channel[i].chid == ptr->chid);
        {
            if(pthread_cancel(thr_channel[i].tid)<0)
            {
                syslog(LOG_ERR,"pthread_cancel");
                return -ESRCH;
            }
            pthread_join(thr_channel[i].tid,NULL);
            thr_channel[i].chid = -1;
            return 0;
        }
        
    }
    syslog(LOG_WARNING,"未找到对应线程");
    return -1;
    
   
}
int thr_channel_destoryall(void)
{
    for(int i=0;i<CHNNR;i++)
    {
        if(thr_channel[i].chid>0)
        {
            if(pthread_cancel(thr_channel[i].tid)<0)
            {
                syslog(LOG_ERR,"pthread_cancel");
                return -1;
            }
            pthread_join(thr_channel[i].tid,NULL);
            thr_channel[i].chid = -1;
        }
    }
    return 0;


}