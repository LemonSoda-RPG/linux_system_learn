#ifndef PROTO_H
#define PROTO_H
#define NAMESIZE 32
#define KEYPATH "/etc/services"
#define KEYPROJ 'k'    //杂质
struct msg_st
{
    long mtype;

    char name[NAMESIZE];
    int math;
    int chinese;
};



#endif