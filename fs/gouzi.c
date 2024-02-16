#include<stdlib.h>
#include<stdio.h>
static void f1()
{
    puts("f1 is working");
}
static void f2()
{
    puts("f2 is working");
}
int main()
{   
    puts("begin");
    atexit(f1);
    atexit(f2);
    atexit(printf("启动钩子函数"));
    puts("end");
    exit(0);
}
