#ifndef CLIENT_H
#define CLIENT_H
#define DEFAULT_PLAYER_CMD  "/usr/sbin/mpg123 -  > /dev/null"
struct client_conf_st
{
    char *rcvport;
    char *mgroup;
    char *player_cmd;
};
extern struct client_conf_st client_conf;

#endif