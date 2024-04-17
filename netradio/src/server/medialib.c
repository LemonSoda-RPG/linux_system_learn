#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <glob.h>
#include <fcntl.h>
#include "medialib.h"
#include "mytbf.h"
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
int mlib_getchnlist(struct mlib_listentry_st*[],int *)
{


}
int mlib_freechnlist(struct mlib_listentry_st*)
{


}
ssize_t mlib_readchn(chid_t,void *,size_t)
{


}