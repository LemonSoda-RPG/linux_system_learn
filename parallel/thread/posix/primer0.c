#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>
#include <string.h>
#define LEFT 300000000
#define RIGHT 300001000
static pthread_t pthl[RIGHT-LEFT+1];
void *ptr;
struct arg{
    int i;
};
//交叉算法计算 池类算法涉及到竞争
static void *func(void *p)
{   
    int i,j,mark;
    i=((struct arg*)p)->i;
    mark = 1;
    for (j = 2;j <= i/2;j++)
    {
        if (i%j == 0)
        {
            mark = 0;
            break;
        }
    }
    if (mark) 
    {
        printf("%d is a primer\n",i);
    }
    pthread_exit(p);
}
int main()
{
    // pthread_t pt;
    struct arg *sti;
    for(int i=LEFT;i<=RIGHT;i++)
    {   
        sti = (struct arg *)malloc(sizeof(*sti));
        if(sti==NULL)
        {
            perror("malloc");
            exit(1);
        }
        sti->i = i;
         // 分配内存给中间变量 
        int err = pthread_create(pthl+i-LEFT,NULL,&func,sti);
        if(err)
        {
            fprintf(stderr,"%s\n",strerror(err));
            for(int k=0;k<RIGHT-LEFT+1;k++)
            {
                if(pthl[k])
                {
                    int num = pthread_join(pthl[k],NULL);
                    if(num)
                    {
                        fprintf(stderr,"%s\n",strerror(num));
                        exit(1);
                    }
                }
            }
            exit(1);
        }
    }
    for(int k=0;k<RIGHT-LEFT+1;k++)
    {
        if(pthl[k])
        {
            int num = pthread_join(pthl[k],&ptr);   //ptr用于接收返回的值
            free(ptr);
            if(num)
            {
                fprintf(stderr,"%s\n",strerror(num));
                exit(1);
            }
        }
    }
    exit(0);
}
