#include "pfunc.h"

int createChildProcess(proInfo *pinfo,int pronum)
{
    int i;
    pid_t pid;
    int fds[2];
    //创建pronum个子进程和pronum个能传输套接字描述符的管道
    for(i=0;i<pronum;i++)
    {
        //创建管道
        socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        //创建子进程
        pid=fork();
        if(!pid)//子进程
        {
            close(fds[1]);//关闭管道写端
            task(fds[0]);//通过管道读端，获取任务
        }
        close(fds[0]);//关闭读端
        pinfo[i].fds=fds[1];//关联父进程和具体的子进程之间的管道写端，等待分配任务
        pinfo[i].pid=pid;//获得子进程pid
        pinfo[i].busy=0;
    }
    return 0;
}

void task(int fds)//执行任务
{
    int new_fd;
    char flag=1;
    while(1)
    {
        recv_fd(fds,&new_fd);//接收任务
        printf("I finish the task,over\n");//执行任务
        close(new_fd);//关闭套接字描述符
        write(fds,&flag,sizeof(char));//向特殊管道写入1通知父进程，任务结束
    }
}
