#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <grp.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <stdlib.h>
#define args_check(arg,num) {if(arg != num) return -1;}
//定义子进程信息结构体
typedef struct{
    pid_t pid;//子进程id
    int fds;  //管道子进程端
    short busy;//子进程状态
}proInfo;
//定义应用层协议结构体（传送的内容）
typedef struct{
    int dataLen; //传送数据长度
    char buf[1000];//传送的内容
}transDatagram;//传送的数据包

#define Download "file"
//tcp链接初始化（socket,bind)
int tcpInit(int*,char*,char*);
//创建子进程
int createChildProcess(proInfo *,int);
//父进程向子进程发送socket_fd
void send_fd(int,int);
//子进程接收父进程发送的socket描述符
void recv_fd(int,int*);
//子进程接收任务
void task(int);
//子进程执行任务
int transFile(int);
