#include<stdlib.h>
#include<stdio.h>
#include<string.h>
int main(int argc,char **argv)
{
    FILE *file;
    file = fopen(argv[1],"r");
    char *lineptr;
    size_t patchsize;
    if(!file)
    {
        perror("打开文件失败");
        exit(1);
    }
    lineptr = NULL;
    patchsize = 0;
    
    while(1)
    {
        if(getline(&lineptr,&patchsize,file)<0)
        {
            break;
        }
        printf("%s\n",lineptr);
        // printf("%d\n",patchsize);
    }
    fclose(file);
    return 0;
}