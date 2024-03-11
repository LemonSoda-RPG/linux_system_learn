#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include <unistd.h>
#define THRNUM 20
#define BUFFERSIZE 1024
#define FNAME "/tmp/out"
static void *func(void* p)
{
    FILE *fp;
    char buffer[BUFFERSIZE];
    fp = fopen(FNAME,"r+");
    if(fp ==NULL)
    exit(1);
    fgets(buffer,BUFFERSIZE,fp);
    fseek(fp,0,SEEK_SET);
    int number = atoi(buffer);
    number+=1;
    sleep(1);
    fprintf(fp,"%d\n",number);
    fclose(fp);
    pthread_exit(NULL);
}
int main()
{
    pthread_t pth_l[THRNUM];
    for(int i=0;i<THRNUM;i++)
    {
        int err = pthread_create(pth_l+i,NULL,&func,NULL);
        if(err)
        {
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }
    for(int i=0;i<THRNUM;i++)
    {
        pthread_join(pth_l[i],NULL);
    }
}