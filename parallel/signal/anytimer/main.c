#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "anytimer.h"
static void f1(void*p)
{
    printf("f1():%s\n",p);
}
static void f2(void*p)
{
    printf("f2():%s\n",p);
}
static void f3(void*p)
{
    printf("f3():%s\n",p);
}
int main()
{
   

    int job1,job2,job3;
    puts("begin!");

    job1 = at_addjob(7,f1,"777");
    if(job1 < 0)
    {

    }

    job2 = at_addjob_repeat(3,f2,"333");
    job3 = at_addjob(4,f3,"444");
    


    at_waitjob(job1);
    at_waitjob(job3);
    // at_waitjob(job2);
    
    puts("end!");
    while(1)
    {
        write(1,".",1);
        sleep(1);
    }
    exit(0);
}