#include "thr_list.h"
#include "server_conf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <syslog.h>
#include <unistd.h>
static int num_list_entry;
static pthread_t tid_list;
struct msg_list_st *entrylistptr;  //每个频道的信息数组 包括所有的信息
static struct mlib_listentry_st *list_entry;
static void* thr_list(void*p)
{
    int totalsize;
    
    struct msg_listentry_st *entryptr; //  保存单个频道的信息  指针 指向所有信息
    int ret; 
    int size;
    totalsize = sizeof(chid_t);  //节目单频道id是0 
    for(int i=0;i<num_list_entry;i++)
    {
        totalsize+=sizeof(struct msg_listentry_st)+strlen(list_entry[i].desc);
    }
    
    entrylistptr = malloc(totalsize);
    if(entrylistptr==NULL)
    {
        syslog(LOG_ERR,"entrylistptr = malloc(totalsize);");
        exit(1);
    }
    entrylistptr->chid = LISTCHNID;  //节目单频道号
    entryptr = entrylistptr->entry;
    syslog(LOG_DEBUG,"频道的数量为： %d",num_list_entry);

    for(int i=0;i<num_list_entry;i++)
    {
        //strlen的大小不包括尾0  而msg_listentry_st中正好有一个char的数组 大小为1  
        size = sizeof(struct msg_listentry_st)+ strlen(list_entry[i].desc);   
        entryptr->chid = list_entry[i].chid;
        strcpy(entryptr->desc,entryptr[i].desc);
        entryptr->len = htons(size);
        syslog(LOG_DEBUG,"entryp len:%d",entryptr->len);

        entryptr = (void*)((char*)entryptr+size);
    }
    while(1)
    {
        syslog(LOG_INFO,"thr_list sndaddr",sndaddr.sin_addr.s_addr);
        ret = sendto(serversd,entrylistptr,totalsize,0,(void*)&sndaddr,sizeof(sndaddr));
        syslog(LOG_DEBUG,"sent content len:%d\n",entrylistptr->entry->len);
        if(ret<0)
        {
            syslog(LOG_WARNING,"sendto");

        }
        else{
            syslog(LOG_DEBUG,"sendto:success");
        }
        sleep(1);
    }


}
int thr_list_create(struct mlib_listentry_st* me,int size)
{   
    num_list_entry = size;
    list_entry = me;
    int err;
    for(int i = 0;i>size;i++)
    {
        syslog(LOG_DEBUG, "list content: chnid:%d, desc:%s\n",
                me[i].chid,me[i].desc);
    }
    err = pthread_create(&tid_list,NULL,thr_list,NULL);
    if(err)
    {
        syslog(LOG_ERR,"pthread_create()");
        exit(1);
    }
    return 0;

}
int thr_list_destory(void)
{
    pthread_cancel(tid_list);
    pthread_join(tid_list,NULL);
    return 0;
}