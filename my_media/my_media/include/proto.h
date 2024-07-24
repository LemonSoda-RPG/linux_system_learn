#ifndef PROTO_H
#define PROTO_H
#include "site_type.h"
#define DEFAULT_MGROUP      "224.2.2.2"                         // 多播组ip
#define DEFAULT_PCVPORT     "8077"                              // 服务端发送端口
#define CHNNR               100                                 // 频道数量
#define LISTCHNID           0                                   // 0号频道用于发送节目单
#define MINCHNID            1                                   // 最小的频道号
#define MAXCHNID            (MINCHNID+CHNNR-1)                  // 最大频道号
#define MSG_CHANNEL_MAX     (65536-20-8)    // 包最大的长度    udp的推荐长度是512 到  64k   包括报头长度 以及udp的报头的长度                  
#define MAX_DATA            (MSG_CHANNEL_MAX-sizeof(chid_t))   // 最大数据大小 
#define MSG_LIST_MAX        (65536-20-8)
#define MAX_ENTRY           (MSG_CHANNEL_MAX-sizeof(chid_t))



// msg_channel_st与msg_list_st 是同等级的

// 我们在网络中传输的就这两种包
// msg_channel_st 和 msg_list_st
// 频道数据包
struct msg_channel_st
{
    chid_t chid;

    // 存储发送的数据
    // 当然 具体是发送多少数据是动态决定的
    // 这个大小取决于音频的质量  码率  如果码率很大  可以设置大一点
    uint8_t data[1];
}__attribute__((packed));   //不需要进行对齐




// 节目单结构体
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
}__attribute__((packed));
#endif