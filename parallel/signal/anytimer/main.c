#include <stdlib.h>
#include <stdio.h>
#include "anytimer.h"

int main()
{
    // f1 = at_jobfunc_t()

    int job1,job2,job3;
    puts("begin!");

    job1 = at_addjob(7,f1,"777");
    if(job1 < 0)
    {

    }

    job2 = at_addjob(3,f2,"333");
    job3 = at_addjob(4,f3,"444");
    
    puts("end!");
    while(1)
    {
        write();
        sleep();
    }
    exit(0);
}