#include <stdio.h>
#include <stdlib.h>
#include <client.h>
#include <unistd.h>
#include <getopt.h>
#include <proto.h>

/*
    -M --mgroup  指定多播放组
    -P --port   指定端口
    -p --player 指定播放器
    -H --help 显示帮助
*/
struct client_conf_st client_conf = {
            .mgroup = DEFAULT_MGROUP,
            .rcvport = DEFAULT_PCVPORT,
            .player_cmd = DEFAULT_PLAYER_CMD
                   };

int main(int argc,char **argv)
{
/*
初始化 
级别：默认值，配置文件，环境变量 命令行参数
参数优先级 命令行参数最优先

*/
    int c=0;
    int index = 0;
    struct option argarr[] = {{"mgroup",1,NULL,'M'},
                                {"port",1,NULL,'P'},
                                {"help",0,NULL,'H'},
                                {"player",1,NULL,'p'},
                                {NULL,0,NULL,0}};
    while(1)
    {
        c = getopt_long(argc,argv,"M:P:p:H:",argarr,&index);
        if(c<0)
            break;
        switch (c)
        {
        case 'P':
            client_conf.rcvport = optarg;
            break;
        case 'M':
            client_conf.mgroup = optarg;
            break;
        case 'p':
            client_conf.player_cmd = optarg;
            break;
        case 'H':
            printf("-M --mgroup  指定多播放组\n
                    -P --port   指定端口\n
                    -p --player 指定播放器\n
                    -H --help 显示帮助\n");
            exit(0);
        default:
            break;
        }
    }

    socket();
    pipe();


    fork();


    // 子进程 调解码器

    // 父进程 从网络收包 发送给子进程
    exit(0);

}