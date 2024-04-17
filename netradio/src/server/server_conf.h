//
// Created by jiacheng on 4/17/24.
//

#ifndef NETRADIO_SERVER_CONF_H
#define NETRADIO_SERVER_CONF_H
#define DEFAULT_MEDIADIR        "/home/jiacheng/linux_system_learn/netradio/media/"
#define DEFAULT_IF              "eth0"


enum{
    RUN_DAEMON = 1,
    RUN_FRONT
};
struct server_conf_st{
    char *rcvport;
    char *mgroup;
    char *media_dir;
    char runmode;
    char *ifname;

};
extern struct server_conf_st server_default_conf;




#endif //NETRADIO_SERVER_CONF_H