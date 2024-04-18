#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <glob.h>
#include <fcntl.h>
#include "medialib.h"
#include "mytbf.h"
#include "server_conf.h"
#include "proto.h"
#include <syslog.h>
#define PATHSIZE    1024
struct channel_context_st
{
    chid_t chid;
    char *desc;
    glob_t mp3glob;
    int pos;
    int fd;
    off_t offset;
    mytbf_t *tbf;
};
struct channel_context_st channel[MAXCHNID+1];

int mlib_getchnlist(struct mlib_listentry_st* result[],int *resnumber)
{
    glob_t globres;
    struct mlib_listentry_st*ptr;
    struct channel_context_st *res;
    int number = 0;
    char path[PATHSIZE];
    for(int i=0;i<MAXCHNID+1;i++)
    {
        channel[i].chid = -1;
    }
    //对路径进行解析
    snprintf(path,PATHSIZE,"%s/*",server_default_conf.media_dir);
    if(glob(path,0,NULL,&globres))
    {
        return -1;
    }
    
    ptr = malloc(sizeof(struct mlib_listentry_st)*globres.gl_pathc);
    if(ptr==NULL)
    {
        syslog(LOG_ERR,"malloc failed");
        exit(1);
    }
    for(int i=0;i<globres.gl_pathc;i++)
    {
        path2entry(globres.gl_pathv[i]);
        number++;
    }
    *resnumber = number;




}
int mlib_freechnlist(struct mlib_listentry_st*)
{


}
ssize_t mlib_readchn(chid_t,void *,size_t)
{


}