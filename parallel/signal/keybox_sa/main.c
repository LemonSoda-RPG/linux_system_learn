#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "tbf/mytbf.h"
#define CPS 	   10  //每秒加10个token  一个token是第一个字符
#define BUFFERSIZE CPS
#define BURST      1024

int main(int argc,char **argv)
{
	int fd1,fd2=1;
	int len,size;
	char buf[BUFFERSIZE];
    struct mytbf_st* tbf;
	if(argc<2)
	{
		fprintf(stderr, "写入错误\n");
		exit(1);
	}
    tbf = mytbf_init(CPS,BURST);
	if(tbf==NULL)
	{
		perror("mytbf_init");
		exit(1);
	}
	do
	{
		fd1 = open(argv[1],O_RDONLY);
		if(fd1<0)
		{	
			if(errno != EINTR)
			{
				perror("open  失败");
				exit(1);
			}
		}
	}while(fd1<0);

	while(1)
	{
		size = mytbf_fetchtoken(tbf,BUFFERSIZE);
		while((len=read(fd1,buf,size))<0)
		{
			if(errno == EINTR)
				continue;
			perror("error");
			break;
		}
		if(len==0)
		{
			break;
		}
        if(size-len>0)
        {
            mytbf_returntoken(tbf,size-len);
        }
		int i=0;
		int pos = 0;

		while(len>0)
		{
			i++;
			int len2 = write(fd2,buf+pos,len);
			if(len2<0)
			{	
				if(errno == EINTR)
					continue;	
				perror("error");
					exit(1);
			}
			pos+=len2;
			len-=len2;
		}
	}
  	close(fd1);  
	printf("\n");
    mytbf_destory(tbf);
	exit(0);
}
