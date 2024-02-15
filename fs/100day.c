#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<time.h>
#include<unistd.h>
#define TIMESIZE 500
int main()
{

    struct tm* tmres;
    char timestr[TIMESIZE];
    time_t stamp;
    time(&stamp);
    
    tmres = localtime(&stamp);
    //将输出写入到timestr当中
    strftime(timestr,TIMESIZE,"%Y-%m-%d",tmres);
    puts(timestr);

    tmres->tm_mday+=100;
    mktime(tmres);
    strftime(timestr,TIMESIZE,"%Y-%m-%d",tmres);
    puts(timestr);



    exit(0);
}