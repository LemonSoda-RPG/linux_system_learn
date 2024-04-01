#ifndef PROTO_H
#define PROTP_H
#define NAMESIZE 11
#define RCVPORT "8866"
#define NAMEMAX 512-8-8
struct msg_st
{
    uint32_t math;
    uint32_t chinese;
    uint8_t name[1];

}__attribute__((packed));

#endif