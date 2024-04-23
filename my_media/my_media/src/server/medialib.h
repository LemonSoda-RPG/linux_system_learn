#ifndef MEDIALIB_H
#define MEDIALIB_H
#include "site_type.h"
#include "proto.h"
#include <unistd.h>
struct mlib_listentry_st
{
    chid_t chid;
    char *desc;
};
int mlib_getchnlist(struct mlib_listentry_st **res,int *ptr);
int mlib_freechnlist(struct mlib_listentry_st* mchn);
ssize_t mlib_readchn(chid_t,void *,size_t p);

#endif