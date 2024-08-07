#ifndef MYFTP_H
#define MYPFT_H

#define KEYPATH "/etc/device"
#define KEYPROJ 'a'
#define DATAMAX 1024

#define PATHMAX 1024

enum{
    MSG_PATH = 1,
    MSG_DATA,
    MSG_EOT
};
// 管道用于传输的结构体
//mtype 因为管道发送和接受的包是未知类型的，
// 为了应对不同的需求，一个管道可能会首发多种不同类型的包
// 因此要用mtype进行区分
typedef struct msg_path_st
{   
    long mtype;
    char path[PATHMAX];
}msg_path_s;
typedef struct msg_data_st
{   
    long mtype;
    int datalen;
    /*
        如果datalen的值大于0  那就是数据包
        如果datalen小于0 那么就是eot包
    */
    char data[DATAMAX];
  
}msg_data_t;


#endif