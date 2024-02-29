#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <glob.h>
// #include <stdio.h>

#define DELIMS " \t\n"
static void parse(char *line,glob_t *globres)
{
    char *tok;
    int i=0;
    while(1)
    {
        tok = strsep(&line,DELIMS);
        if(tok == NULL)
            break;
        if(tok[0]=='\0')
            continue;
        glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,globres);
        i=1;
    }
}
int main()
{
    // printf("\n");
    pid_t pid;
    char* cmd = NULL;
    size_t cmdsize = 0;
    glob_t globres;
    while(1)
    {   
        printf("myshell$ ");
        
        if(getline(&cmd,&cmdsize,stdin)<0)
            break;   //获取命令行
        parse(cmd,&globres);
        if(0)       //如果是内部命令 不存在二进制
        {

        }
        else
        {
            // 如果是外部命令 
            pid = fork();  //在父进程里 会返回子进程的pid  子进程会返回0
            if(pid<0)  //出错
            {
                perror("fork");
                exit(1);
            }
            if(pid==0)  //说明是子进程
            {
                execvp(globres.gl_pathv[0],globres.gl_pathv);
                perror("execvp");
                exit(1);
            }
            else //如果是父进程  则等待收尸
            {
                wait(NULL);
            }
        }
    }
    exit(0);
}
