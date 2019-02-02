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

typedef struct{
    pid_t pid;
    int fds;
    short busy;
}proInfo;

int tcpInit(int*,char*,char*);
int createChildProcess(proInfo *,int);
void task(int);
void send_fd(int,int);
void recv_fd(int,int*);
