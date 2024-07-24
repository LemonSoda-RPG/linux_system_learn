#ifndef MEDIALIB_H
#define MEDIALIB_H
#include "site_type.h"
#include "proto.h"
#include <unistd.h>


// 之后我们会将这个数据回填到用于网络传输的结构体当中
struct mlib_listentry_st
{
    chid_t chid;
    char *desc;
};
// 获取频道信息
int mlib_getchnlist(struct mlib_listentry_st **res,int *ptr);
int mlib_freechnlist(struct mlib_listentry_st* mchn);
ssize_t mlib_readchn(chid_t,void *,size_t p);

#endif