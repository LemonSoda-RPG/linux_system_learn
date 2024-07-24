//
// Created by jiacheng on 4/17/24.
//

#ifndef NETRADIO_SERVER_CONF_H
#define NETRADIO_SERVER_CONF_H
#define DEFAULT_MEDIADIR        "/home/jc/桌面/my_media/media"
#define DEFAULT_IF              "eth0"

// 守护进程 还是前台运行
enum{
    RUN_DAEMON = 1,
    RUN_FRONT
};

// 保存命令行参数信息
struct server_conf_st{
    char *rcvport;
    char *mgroup;
    char *media_dir;
    char runmode;
    char *ifname;

};


extern struct server_conf_st server_default_conf;


extern int serversd;  // 套接字
extern struct sockaddr_in sndaddr;  // 发送目标的信息

#endif //NETRADIO_SERVER_CONF_H
