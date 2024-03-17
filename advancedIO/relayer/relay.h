#ifndef RELAY_h
#define RELAY_h
#define JOB_MAX 10000
#include<stdint.h>
// #include<

enum{
    STATE_RUN = 1,
    STATE_CANELED,
    STATE_OVER
};
enum {
    STATE_R = 1, // 读
    STATE_W, // 写
    STATE_Ex, // 异常
    STATE_T // 终止
};

struct relay_st
{
    /* data */
    int stat;
    int fd1;
    int fd2;
    int64_t count12;
    int64_t count21;
};


int cancel_job(int id);
/*
成功
有以下几种失败的可能
参数非法
列表已满
内存有误
*/
int add_job(int fd1,int fd2);

/*
参数非法
任务不存在
*/

int wait_job(int id, struct relay_st* st);

int stat_job(int id, struct relay_st* st);


#endif