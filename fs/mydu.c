#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<glob.h>
#include <sys/stat.h>
static int64_t mydu(const char *pathname)
{
    // 首先判断path是否为目录
    struct stat statres;
    if(lstat(pathname,&statres)<0)
    {
        perror("lstat");
        exit(1);
    }
    else
    {
        if(!S_ISDIR(statres.st_mode))
        {
            // 非目录
            return statres.st_blocks;
        }

    }
}
int main(int argc,char *argv[])
{
    if(argc<2)
    {
        perror("argc error!");
        exit(1);
    }

    printf("文件大小为%lld\n",(mydu(argv[1])/2)*1024);

    

    exit(0);

}