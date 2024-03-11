#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_func(void *p)
{
    puts(p);
}

static void *func(void* p)
{
    puts("thread is working");
    pthread_cleanup_push(cleanup_func,"clean:1");
    pthread_cleanup_push(cleanup_func,"clean:2");
    pthread_cleanup_push(cleanup_func,"clean:3");
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}


int main()
{

    pthread_t pt;
    puts("begin");
    int err = pthread_create(&pt,NULL,&func,NULL);
    if(err)
    {
        fprintf(stderr,"%s\n",strerror(err));
        exit(1);
    }
    pthread_join(pt,NULL);
    puts("end");
    exit(0);
}