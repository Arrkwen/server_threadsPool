#include "pfunc.h"

//子进程
void task(int fds)//执行任务
{
    int new_fd;
    char flag=1;
    if(3==fds)
    {
        exit(0);
    }
    while(1)
    {
        recv_fd(fds,&new_fd);//接收任务i
        transFile(new_fd);//传送文件
        close(new_fd);//关闭套接字描述符
        write(fds,&flag,sizeof(char));//向特殊管道写入1通知父进程，任务结束
    }
}

