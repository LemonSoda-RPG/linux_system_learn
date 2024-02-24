#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
/*
exec函数族实例
*/
int main()
{

    printf("begin\n");
    fflush(NULL);
    execlp("date","date","+%s",NULL);
    perror("execlp");
    exit(1);
    puts("end");
    exit(0);
}