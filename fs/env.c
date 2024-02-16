#include<stdio.h>
#include<stdlib.h>
int main()
{

    setenv("LANG","en",1);
    puts(getenv("LANG"));
    return 0;
    
}