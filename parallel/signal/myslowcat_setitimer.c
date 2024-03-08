#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#define CPS 	   10
#define BUFFERSIZE CPS
static volatile int  loop = 0; 
static void alrm_handler(int s)
{	
	loop = 1;
}
int main(int argc,char **argv)
{
	int fd1,fd2=1;
	struct itimerval itst;
	// struct itimerval itstold;
	int len;
	char buf[BUFFERSIZE];
	if(argc<2)
	{
		fprintf(stderr, "写入错误\n");
		exit(1);
	}

	signal(SIGALRM,alrm_handler);
	// alarm(1);
	itst.it_interval.tv_sec =1;
	itst.it_interval.tv_usec = 0;
	itst.it_value.tv_sec = 1;
	itst.it_value.tv_usec = 0;
	// it_value的值耗尽之后 发送信号 且it_interval的值会赋值给it_value
	// 
	if(setitimer(ITIMER_REAL,&itst,NULL)<0)
	{
		perror("setitimer()");
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
		while(!loop)
			pause();
		
		// alarm(1);
		loop = 0;
		
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
