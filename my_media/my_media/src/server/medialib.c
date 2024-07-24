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
    chid_t chid;        // 频道id
    char *desc;         // 介绍
    glob_t mp3glob;     // 解析的数据
    int pos;            // 当前播放歌曲的下标
    int fd;             // 打开文件的fd
    off_t offset;       // 当前播放的位置
    mytbf_t *tbf;       // 流量控制 
};
// 0下标用于节目单线程
struct channel_context_st channel[MAXCHNID+1];


static struct channel_context_st *path2entry(char*path)
{
    syslog(LOG_INFO,"current path:%s",path);
    char pathstr[PATHSIZE] = {'\0'};
    char linebuf[LINEBUFSIZE];
    FILE *fp;
    // 用于存储当前频道的信息
    struct channel_context_st*me;
    // 静态定义  只生效一次
    static chid_t curr_id = MINCHNID;
    
    // 将字符串进行拼接
    strcat(pathstr,path);
    strcat(pathstr,"/desc.txt");
    fp = fopen(pathstr,"r");  //打开描述文件
    syslog(LOG_INFO,"desc path:%s",pathstr);
    if(fp==NULL)
    {
        syslog(LOG_WARNING,"%s is not a channel",path);
        return NULL;
    }
    //读取描述文件到linebuf
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
    // 此时pathstr代表当前目录的所有mp3的通配符
    // 对pathstr 进行解析
    // 将解析出的信息填到mp3glob
    if(glob(pathstr,0,NULL,&me->mp3glob)!=0)
    {
        curr_id++;
        syslog(LOG_ERR, "%s is not a channel dir(can not find mp3 files", path);
        free(me);
        return NULL;
    }
    me->pos = 0;
    me->offset =0;
    // 打开mp3文件
    me->fd = open(me->mp3glob.gl_pathv[me->pos],O_RDONLY);
    if(me->fd<0)
    {
        syslog(LOG_WARNING,"%s open failed.",me->mp3glob.gl_pathv[me->pos]);
        free(me);
        return NULL;

    }
    // 设置频道号
    me->chid = curr_id;
    curr_id++;
    return me;


}

// 这里因为我们是在函数的内部对指针分配了内存  所以传入的是指针的指针
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
    
    
    // 将路径写入到path
    snprintf(path,PATHSIZE,"%s/*",server_default_conf.media_dir);
    if(glob(path,0,NULL,&globres))
    {   
        printf("解析失败\n");
        return -1;
    }
    //这里我们解析出的globres.gl_pathc的数量 可能包含空文件夹  
    //因此我们这里分配的ptr内存有可能是比较大的  之后可以动态的减小内存  
    // 我们解析出的目录信息都保存在了globres
    // globres.gl_pathc的大小就是我们的频道数目

    // mlib_listentry_st  也就是ptr中保存了频道的id以及描述  其中描述是指针，指向我们真正的保存描述的结构体
    // 这里我们分配的内存是比较大的  因为是以每一个频道文件夹都有效为前提分配的内存
    // 我们处理过程会进行计数  之后重新分配内存大小
    ptr = malloc(sizeof(struct mlib_listentry_st)*globres.gl_pathc);
    if(ptr==NULL)
    {
        syslog(LOG_ERR,"malloc failed");
        exit(1);
    }
    //这里我们解析出的globres.gl_pathc的数量 可能包含空文件夹
    for(int i=0;i<globres.gl_pathc;i++)
    {
        // 遍历每一个文件夹  也就是每一个频道的内容
        res = path2entry(globres.gl_pathv[i]);
        if(res==NULL)
        {
            syslog(LOG_ERR,"path2entry() failed");
            continue;
        }
        syslog(LOG_INFO,"path2entry() return: %d %s.",res->chid,res->desc);
        // 将数据写入到 完整的结构体当中
        memcpy(channel+(res->chid),res,sizeof(*res));
        ptr[number].chid = res->chid;
        // 分配一块新的内存  将res->desc指向的内容复制到新内存中 并返回地址
        ptr[number].desc = strdup(res->desc);
        number++;
        
    }
    // 这里重新分配内存的大小 因为可能有的文件夹会无效
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
        // pos记录当前频道播放的哪首音乐   既然要切歌  那就++
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

// 读取当前频道  会在thr_channel被调用
/**
 * @param chid 频道id
 * @param buf  数据要写入的缓存
 * @param size 读取的大小
 */
ssize_t mlib_readchn(chid_t chid,void *buf,size_t size)
{

    // 读取当前频道
    printf("chid is %d\n",chid);
    int tbfsize;
    int len;
    // int next_ret = 0;
    // 取令牌
    tbfsize = mytbf_fetchtoken(channel[chid].tbf,size);
    syslog(LOG_INFO, "current tbf():%d", mytbf_checktoken(channel[chid].tbf));
    while(1){
        // pread支持从指定的位置开始读取数据且不会影响文件指针  这有利于多线程读取同一个文件 互不影响 
        // channel[chid].fd 当前频道打开的文件
        // buf  写入的缓存
        // tbfsize 读取的大小
        // channel[chid].offset  读取的位置
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
            // 读取成功  更新偏移量
            channel[chid].offset+=len;
            struct stat buf;

            // 通过偏移量以及文件的总大小  获取当前的进度 
            fstat(channel[chid].fd,&buf);
            //获取进度
            syslog(LOG_DEBUG, "epoch : %f%%",(channel[chid].offset) / (1.0*buf.st_size)*100);
            break;
        }
    }
    // 令牌没用完 归还
    if(tbfsize-len>0)
    {
        mytbf_returntoken(channel[chid].tbf,tbfsize - len);
    }
    printf("current chid:%d\n",chid);
    printf(" 读取到的长度为%d\n",len);
    return len;
}