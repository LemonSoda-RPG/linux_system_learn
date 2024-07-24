#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <glob.h>
#include <string.h>
#include <fcntl.h>
#include "medialib.h"
#include <sys/stat.h>
#include "mytbf.h"
#include "server_conf.h"
#include "proto.h"
#include <syslog.h>
#define PATHSIZE    1024
#define LINEBUFSIZE    1024
#define MP3_BITRATE 320*1024

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


static struct channel_context_st *path2entry(char*path)
{
    syslog(LOG_INFO,"current path:%s",path);
    char pathstr[PATHSIZE] = {'\0'};
    char linebuf[LINEBUFSIZE];
    FILE *fp;
    struct channel_context_st*me;
    static chid_t curr_id = MINCHNID;
    

    strcat(pathstr,path);
    strcat(pathstr,"/desc.txt");
    fp = fopen(pathstr,"r");  //打开描述文件
    syslog(LOG_INFO,"desc path:%s",pathstr);
    if(fp==NULL)
    {
        syslog(LOG_WARNING,"%s is not a channel",path);
        return NULL;
    }
    if(fgets(linebuf,LINEBUFSIZE,fp)==NULL)
    {
        syslog(LOG_INFO,"%s is not a channel",path);
        fclose(fp);
        return NULL;
    } 
    fclose(fp);

    me = malloc(sizeof(struct channel_context_st));
    if(me==NULL)
    {
        syslog(LOG_ERR,"malloc()");
        return NULL;
    }

    me->tbf = mytbf_init(MP3_BITRATE/8*2,MP3_BITRATE / 8 * 5);
    printf("mytbf_init()\n");
    if(me->tbf==NULL)
    {
        syslog(LOG_ERR,"mytbf_init()");
        free(me);
        return NULL;
    }

    me->desc = strdup(linebuf);
    strncpy(pathstr,path,PATHSIZE); //这里减一是为了给 "\0"留位置
    strncat(pathstr,"/*.mp3",PATHSIZE-1);//这里减一是为了给 "\0"留位置
    if(glob(pathstr,0,NULL,&me->mp3glob)!=0)
    {
        curr_id++;
        syslog(LOG_ERR, "%s is not a channel dir(can not find mp3 files", path);
        free(me);
        return NULL;
    }
    me->pos = 0;
    me->offset =0;
    me->fd = open(me->mp3glob.gl_pathv[me->pos],O_RDONLY);
    if(me->fd<0)
    {
        syslog(LOG_WARNING,"%s open failed.",me->mp3glob.gl_pathv[me->pos]);
        free(me);
        return NULL;

    }
    me->chid = curr_id;
    curr_id++;
    return me;


}
int mlib_getchnlist(struct mlib_listentry_st** result,int *resnumber)
{
    glob_t globres;
    struct mlib_listentry_st *ptr;
    struct channel_context_st *res;
    int number = 0;
    char path[PATHSIZE];
    for(int i=0;i<MAXCHNID+1;i++)
    {
        channel[i].chid = -1;
    }
    //对路径进行解析
    printf("%s/*",server_default_conf.media_dir);

    snprintf(path,PATHSIZE,"%s/*",server_default_conf.media_dir);
    if(glob(path,0,NULL,&globres))
    {   
        printf("解析失败\n");
        return -1;
    }
    //这里我们解析出的globres.gl_pathc的数量 可能包含空文件夹  
    //因此我们这里分配的ptr内存有可能是比较大的  之后可以动态的减小内存  
    ptr = malloc(sizeof(struct mlib_listentry_st)*globres.gl_pathc);
    if(ptr==NULL)
    {
        syslog(LOG_ERR,"malloc failed");
        exit(1);
    }
    //这里我们解析出的globres.gl_pathc的数量 可能包含空文件夹
    for(int i=0;i<globres.gl_pathc;i++)
    {
        res = path2entry(globres.gl_pathv[i]);
        if(res==NULL)
        {
            syslog(LOG_ERR,"path2entry() failed");
            continue;
        }
        syslog(LOG_INFO,"path2entry() return: %d %s.",res->chid,res->desc);
        memcpy(channel+res->chid,res,sizeof(*res));
        ptr[number].chid = res->chid;
        ptr[number].desc = strdup(res->desc);
        number++;
        
    }
    *result = realloc(ptr,sizeof(struct mlib_listentry_st) * number);
    if(*result ==NULL)
    {
        syslog(LOG_ERR,"realloc failed");
        exit(1);
    }
    *resnumber = number;
    return 0;

}
int mlib_freechnlist(struct mlib_listentry_st* ptr)
{
    free(ptr);
    return 0;

}
static int open_next(chid_t chid)
{


    for(int i = 0;i<channel[chid].mp3glob.gl_pathc;i++)
    {
        channel[chid].pos++;
        if(channel[chid].pos == channel[chid].mp3glob.gl_pathc)
        {
            printf("文件播放完毕，列表循环\n");
            channel[chid].pos = 0;
        }
        close(channel[chid].fd);
        channel[chid].fd = open(channel[chid].mp3glob.gl_pathv[channel[chid].pos],O_RDONLY);
        if(channel[chid].fd<0)
        {
            syslog(LOG_WARNING,"open()");
        }
        else{
            printf("打开新文件了\n");
            channel[chid].offset = 0;
            return 0;
        }
    }
    syslog(LOG_ERR, "None of mp3 in channel %d id available.全部打开失败了", chid);
    return -1;
}
ssize_t mlib_readchn(chid_t chid,void *buf,size_t size)
{
    printf("chid is %d\n",chid);
    int tbfsize;
    int len;
    // int next_ret = 0;
    tbfsize = mytbf_fetchtoken(channel[chid].tbf,size);
    syslog(LOG_INFO, "current tbf():%d", mytbf_checktoken(channel[chid].tbf));
    while(1){
        len = pread(channel[chid].fd,buf,tbfsize,channel[chid].offset);
        if(len<0)
        {
            syslog(LOG_WARNING, "读取失败 切歌！！！！media file %s pread():%s",
                channel[chid].mp3glob.gl_pathv[channel[chid].pos],
                strerror(errno));
            open_next(chid);  //这首歌读取失败 接着读取下一首歌
        }
        else if(len==0)
        {
            //这首歌读完了  接着读下一首歌
            open_next(chid);
            //todo 如果open_next 返回的是-1 那就说明这个频道已经完了
            break;

        }
        else{
            channel[chid].offset+=len;
            struct stat buf;
            fstat(channel[chid].fd,&buf);
            //获取进度
            syslog(LOG_DEBUG, "epoch : %f%%",(channel[chid].offset) / (1.0*buf.st_size)*100);
            break;
        }
    }
    if(tbfsize-len>0)
    {
        mytbf_returntoken(channel[chid].tbf,tbfsize - len);
    }
    printf("current chid:%d\n",chid);
    printf(" 读取到的长度为%d\n",len);
    return len;
}