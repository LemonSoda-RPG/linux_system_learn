#ifndef PROTO_H
#define PROTO_H
#include "site_type.h"
#define DEFAULT_MGROUP      "224.2.2.2"
#define DEFAULT_PCVPORT     "8077"
#define CHNNR               100
#define LISTCHNID           0
#define MINCHNID            1 
#define MAXCHNID            (MINCHNID+CHNNR-1)
#define MSG_CHANNEL_MAX     (65536-20-8)
#define MAX_DATA            (MSG_CHANNEL_MAX-sizeof(chid_t))
#define MSG_LIST_MAX        (65536-20-8)
#define MAX_ENTRY           (MSG_CHANNEL_MAX-sizeof(chid_t))
struct msg_channel_st
{
    chid_t chid;
    uint8_t data[1];
}__attribute__((packed));

struct msg_listentry_st
{
    chid_t chid;
    uint16_t len;
    uint8_t desc[1];  //8位 一个字节
}__attribute__((packed));

struct msg_list_st
{
    //节目单
    chid_t chid;
    struct msg_listentry_st entry[1]; 
};

#endif