#include<stdio.h>
#include<stdlib.h>
#include <sys/ipc.h>
#include<string.h>
#include <sys/msg.h>
#include "proto.h"

int main()
{
    struct msg_st sbuf;
    key_t key;
    key = ftok(KEYPATH,KEYPROJ);
    if(key<0)
    {
        perror("");
        exit(1);
    }
    int msgid = msgget(key,0);  //因为他不需要创建，所以是0
    
    sbuf.mtype = 1;
    sbuf.chinese = rand()%100; 
    strcpy(sbuf.name,"alan");
    sbuf.math = rand()%100; 
    

    if(msgsnd(msgid,&sbuf,sizeof(sbuf)-sizeof(long),0)<0)
    {
        perror("");
        exit(1);
    }
    puts("ok");
    exit(0);
}