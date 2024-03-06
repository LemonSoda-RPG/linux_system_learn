#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFERSIZE 1
// #include <sys/types.h>
// #include<sys/stat.h>
int main(int argc,char **argv)
{
	if(argc<3)
	{
		fprintf(stderr, "写入错误\n");
		exit(1);
	}
	char buf[BUFFERSIZE];
	int fd1 = open(argv[1],O_RDONLY);
	if(fd1<0)
	{
		perror("open  失败");
		exit(1);
	}
	int fd2 = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0600);
	if(fd2<0)
	{
		perror("open  失败");
		close(fd1);
		exit(1);
	}
	while(1)
	{
		int len = read(fd1,buf,BUFFERSIZE);
		if(len<0)
		{
			perror("error");
			break;
		}
		else if(len==0)
		{
			break;
		}
		printf("%s\n",buf);
		
		int len2 = write(fd2,buf,len);
		if(len2!=len)
		{
			perror("error");
			break;
		}

	}
	close(fd1);
	close(fd2);
	exit(0);
}
