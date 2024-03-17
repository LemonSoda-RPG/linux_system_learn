#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define TTY1 "/dev/tty3"
#define TTY2 "/dev/tty4"
#define BUFSIZE 1024

// 有限状态机的状态枚举
enum {
    STATE_R = 1, // 读
    STATE_W, // 写
    STATE_Ex, // 异常
    STATE_T // 终止
};

struct fsm_st {
    int state;
    int sfd;
    int dfd;
    int len; // 读取到的数据
    int pos; // buf的偏移量
    char buf[BUFSIZE]; // 缓冲区
    char *errstr; // 报错信息
};

/* 状态机驱动 */
static void fsm_driver(struct fsm_st *fsm) {
    int ret;
    // 根据当前状态，决定下一步动作
    switch(fsm->state) {
        // 当前状态为读状态
        case STATE_R:
            fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
            if(fsm->len == 0) { // 读完文件
                fsm->state = STATE_T;
            }else if(fsm->len < 0) {
                if(errno == EAGAIN) { // 数据没有准备好
                    fsm->state = STATE_R;
                } else { // 真错
                    fsm->errstr = "read()";
                    fsm->state = STATE_Ex;
                }
            }else { // 转换为写状态
                fsm->pos = 0;
                fsm->state = STATE_W;
            }
            break;
        // 当前状态为写状态
        case STATE_W:
            ret = write(fsm->dfd, fsm->buf + fsm->pos, fsm->len);
            if(ret < 0) {
                if(errno == EAGAIN) {
                    fsm->state = STATE_W;
                } else {
                    fsm->errstr = "write()";
                    fsm->state = STATE_Ex;
                }
            }else {
                fsm->pos += ret;
                fsm->len -= ret;
                if(fsm->len == 0) { // 写够len个字节
                    fsm->state = STATE_R;
                } else { // 没有写够len个字节
                    fsm->state = STATE_W;
                }
            }
            break;
        case STATE_Ex:
            perror(fsm->errstr);
            fsm->state = STATE_T;
            break;
        case STATE_T:
            break;
        default:
            abort();
            break;
    }
}

static void relay(int fd1, int fd2) {
    int fd1_save, fd2_save;
    struct fsm_st fsm12, fsm21;
    // 获取文件状态选项
    fd1_save = fcntl(fd1, F_GETFL);
    // 设置文件状态选项，添加非阻塞模式
    fcntl(fd1, F_SETFL, fd1_save|O_NONBLOCK);

    fd2_save = fcntl(fd2, F_GETFL);
    fcntl(fd2, F_SETFL, fd2_save|O_NONBLOCK);
    // 设置状态机
    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;
	// 轮询
    while(fsm12.state != STATE_T || fsm21.state != STATE_T) {
        fsm_driver(&fsm12);
        fsm_driver(&fsm21);
    }

    // 恢复用户设置的文件状态
    fcntl(fd1, F_SETFL, fd1_save);
    fcntl(fd2, F_SETFL, fd2_save);
}

// 模拟用户的设置
int main(void) {
    int fd1, fd2; // 左设备和右设备
    // 假设用户不以非阻塞的方式打开文件
    if((fd1 = open(TTY1, O_RDWR)) < 0) {
        perror("open()");
        exit(1);
    }
    write(fd1,"TTY1\n",5);
    // 假设用户以非阻塞的方式打开文件
    if((fd2 = open(TTY1, O_RDWR|O_NONBLOCK)) < 0) {
        perror("open()");
        exit(1);
    }
    write(fd2,"TTY2\n",5);
    relay(fd1, fd2);
    close(fd2);
    close(fd1);

    exit(0);
}