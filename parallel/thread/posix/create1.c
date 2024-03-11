#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void *func(void* p)
{
    puts("thread is working");
    pthread_exit(NULL);
}
int main()
{
    pthread_t pt;
    puts("begin");
    int err = pthread_create(&pt,NULL,&func,NULL);

    if(err)
    {
        fprintf(stderr,"pthread_create:%s\n",strerror(err));
        exit(1);
    }
    pthread_join(pt,NULL);
    puts("end");
    exit(0);
}