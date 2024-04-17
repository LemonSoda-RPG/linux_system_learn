#ifndef MEDIALIB_H
#define MEDIALIB_H
#include "site_type.h"
#include "proto.h"
struct mlib_listentry_st
{
    chid_t chid;
    char *desc;
};
int mlib_getchnlist(struct mlib_listentry_st*[],int *);
int mlib_freechnlist(struct mlib_listentry_st*);


#endif