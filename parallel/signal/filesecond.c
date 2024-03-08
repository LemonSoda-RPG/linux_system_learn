#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include <stdint.h>
//通过计时计算5秒中能++多少次
int main()
{
    time_t end;
    int64_t count =0;
    end = time(NULL)+5;
    while(time(NULL)<=end)
    {
        count ++;
    }
    printf("%lld\n",count);
    exit(0);
}