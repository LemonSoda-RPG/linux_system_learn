#ifndef MYPIPE_H
#define MYPIPE_H
#define PIPESIZE      1024
#define MYPIPE_READ   0x00000001UL
#define MYPIPE_WRITER 0x00000002UL
typedef void mypipe_st;


mypipe_st *mypipe_init(void);
int mypipe_read(mypipe_st*,void *buf,size_t size);
int mypipe_write(mypipe_st*,void *buf,size_t size);
int mypipe_destroy(mypipe_st* );



#endif