#ifndef THR_CHANNEL_H
#define THR_CHANNEL_H
#include "medialib.h"
int thr_channel_create(struct mlib_listentry_st*);
int thr_channel_destory(struct mlib_listentry_st*);
int thr_channel_destoryall(void);

#endif