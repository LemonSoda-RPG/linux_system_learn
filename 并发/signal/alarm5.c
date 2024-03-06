#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<stdint.h>
#include <signal.h>
int loop = 1; 
static void alrm_handle(int s)
{
    loop = 0;
}
int main()
{
    int64_t count = 0;
    alarm(5);
    signal(SIGALRM,alrm_handle);
    while(loop)
    {
        count++;
    }
    printf("%lld\n",count);
    
    exit(0);
}