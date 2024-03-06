#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
int main()
{
    alarm(10);
    // alarm();

    while(1)
    {
        printf("haha1\n");
        pause();
        printf("haha2\n");
    }
    exit(0);
}