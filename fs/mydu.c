#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <glob.h>
#include <sys/stat.h>
#define PATHSIZE 1024 

static int noloop(const char *s)
{
    char* pos;
    pos = strrchr(s,'/');
    if(pos == NULL)
        exit(1);
    if(strcmp(pos+1,".")==0||strcmp(pos+1,"..")==0)
    {
        return 0;
    }
    else
        return 1;
}

static int64_t mydu(const char *pathname)
{
    // 首先判断path是否为目录
    
    static struct stat statres;
    char nextpath[PATHSIZE];
    if(lstat(pathname,&statres)<0)
    {
        perror("lstat");
        exit(1);
    }

    if(!S_ISDIR(statres.st_mode))
    {
        // 非目录
        return statres.st_blocks;
    }

    int64_t sum = 0;
    glob_t globres;
    strncpy(nextpath,pathname,PATHSIZE);
    // 匹配文件
    strncat(nextpath,"/*",PATHSIZE);
    if(glob(nextpath,0,NULL,&globres)<0)
    {
        fprintf(stderr,"glob error");
        exit(1);          
    }
    strncat(nextpath,"/.*",PATHSIZE);
    if(glob(nextpath,GLOB_APPEND,NULL,&globres)<0)
    {
        fprintf(stderr,"glob error");
        exit(1);          
    }
    sum = statres.st_blocks;


    printf("%d\n",i);
    for(int i=0;i<globres.gl_pathc;i++)
    {
        if(noloop(globres.gl_pathv[i]))
            sum += mydu(globres.gl_pathv[i]);
    }
    
    return sum;

        
    
}
int main(int argc,char *argv[])
{
    if(argc<2)
    {
        perror("argc error!");
        exit(1);
    }

    printf("文件大小为 %lld k\n",(mydu(argv[1])/2));
    exit(0);

}