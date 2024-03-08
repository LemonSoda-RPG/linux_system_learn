#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define BUFFERSIZE 1
// #include <sys/types.h>
// #include<sys/stat.h>
int main(int argc,char **argv)
{
  
	if(argc<2)
	{
		fprintf(stderr, "写入错误\n");
		exit(1);
	}
	int fd1,fd2=1;
	char buf[BUFFERSIZE];
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
		int len = read(fd1,buf,BUFFERSIZE);
		if(len<0)
		{
			if(errno == EINTR)
				continue;
			perror("error");
			break;
		}
		else if(len==0)
		{
			break;
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
		// printf("-----------------第%d次打印------------------\n",i);
	}
  	close(fd1);  
	exit(0);
}
