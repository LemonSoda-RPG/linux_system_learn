#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    pthread_t pt;
    pt = pthread_self();
    printf("%lld\n",pt);
    exit(0);
}