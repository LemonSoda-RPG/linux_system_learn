#include<stdio.h>
#include<stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "proto.h"

int main()
{
    struct msg_st rbuf;
    key_t key;
    key = ftok(KEYPATH,KEYPROJ);
    if(key<0)
    {
        perror("ftok");
        exit(1);
    }

    int msgid = msgget(key,IPC_CREAT|0600);  //创建并指定权限
    if(msgid<0)
    {
        perror("ftok");
        exit(1);
    }
    while(1)
    {
        if(msgrcv(msgid,&rbuf,sizeof(rbuf)-sizeof(long),0,0)<0)
        {
            perror("error");
            exit(1);        
        }
        printf("name = %s\n",rbuf.name);
        printf("math = %d\n",rbuf.math);
        printf("chinese = %d\n",rbuf.chinese);
    }
    msgctl(msgid,IPC_RMID,NULL);  //销毁不需要继续传参


    exit(0);
}