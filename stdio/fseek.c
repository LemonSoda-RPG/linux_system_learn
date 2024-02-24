#include<stdio.h>
#include<stdlib.h>

// int hahaha(int a,int b);


int main1()
{
    // hahaha(3,4);
    printf("加法实现成功");
    return 0;
}

int hahaha(int a,int b)
{
    int c = a+b;
    printf("a+b=%d\n",c);
    exit(0);
    return 0;
}

int main()
{
    // printf("while 之前");
    // fflush(NULL);
    int i=5;
    while(i--)
    {
        printf("while中  ");
        fflush(NULL);
    }
    // printf("\n");
    while(1);
    printf("while 之后");
    fflush(NULL);
    return 0;
}