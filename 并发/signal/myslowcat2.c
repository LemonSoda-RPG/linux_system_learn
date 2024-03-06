#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#define CPS 	   10
#define BUFFERSIZE CPS
#define BURST      100
static volatile int  loop = 0; 
static volatile int  token = 0; 

static void alrm_handler(int s)
{	
	alarm(1);
	token++;
	if(token>BURST)
		token = BURST;
}
int main(int argc,char **argv)
{
	int fd1,fd2=1;
	int len;
	char buf[BUFFERSIZE];
	if(argc<2)
	{
		fprintf(stderr, "写入错误\n");
		exit(1);
	}

	signal(SIGALRM,alrm_handler);
	alarm(1);
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
		while(token<=0)
			pause();
		token--;
		while((len=read(fd1,buf,BUFFERSIZE))<0)
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
	exit(0);
}
