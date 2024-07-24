#ifndef CLIENT_H
#define CLIENT_H
#define DEFAULT_PLAYER_CMD  "/usr/bin/mpg123 -  > /dev/null"
struct client_conf_st
{
    char *rcvport;      // 接收数据用端口
    char *mgroup;       // 多播组
    char *player_cmd;   // 播放软件
};
extern struct client_conf_st client_conf;

#endif
