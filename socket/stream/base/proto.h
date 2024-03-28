#ifndef PROTO_H
#define PROTP_H
#define NAMESIZE 11
#define RCVPORT "8866"
struct msg_st
{
    char name[NAMESIZE];
    int math;
    int chinese;

}__attribute__((packed));




#endif