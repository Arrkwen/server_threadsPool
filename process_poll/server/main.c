#include "pfunc.h"

int exitfds[2];

void sigfunc(int p)
{
    //当停止服务的信号来世，对无名管道进行写，代表要让子进程有序对出
    write(exitfds[1],&p,1);
}

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
            signal(SIGQUIT,task);
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

int main(int argc ,char **argv)
{
    int i,j;
    int sfd;
    int new_fd;
    int readyset;
    char flag;
    //创建无名管道，用于通知进程有序退出
    pipe(exitfds);
    signal(SIGUSR1,sigfunc);
    //初始化子进程空间
    args_check(argc,4);
    int childProNum=atoi(argv[3]);
    proInfo *pinfo=(proInfo*)calloc(childProNum,sizeof(proInfo));
    //创建子进程 
    createChildProcess(pinfo,childProNum);
    //tcp链接初始化
    tcpInit(&sfd,argv[1],argv[2]);
    listen(sfd,childProNum);
    //epoll创建活动epfd
    int epfd=epoll_create(1);
    struct epoll_event event,*evs;
    evs=(struct epoll_event*)calloc(childProNum+1,sizeof(struct epoll_event));
    //epoll注册sfd 
    event.events=EPOLLIN;
    event.data.fd=sfd;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
    if(-1==ret)
    {
        perror("epoll_ctl");
        return -1;
    }
    //epoll注册所有子进程的读端
    for(i=0;i<childProNum;i++)
    {
        event.data.fd=pinfo[i].fds;
        ret=epoll_ctl(epfd,EPOLL_CTL_ADD,pinfo[i].fds,&event);
        if(-1==ret)
        {
            perror("epoll_ctl");
            return -1;
        }

    }
    //epoll注册退出管道的读端
    event.data.fd=exitfds[0];
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,exitfds[0],&event);
    if(-1==ret)
    {
        perror("epoll_ctl");
        return -1;
    }

    printf("start server:\n");
    while(1)
    {
        //清空evs,获取可读的管道
        memset(evs,0,sizeof(struct epoll_event));
        readyset=epoll_wait(epfd,evs,childProNum+1,-1);
        //便利所有管道
        for(i=0;i<readyset;i++)
        {
            //如果可读管道为sfd,说明有新的客服端连接
            if(evs[i].data.fd==sfd)
            {
                //接收连接，并获取套接字描述符
                new_fd=accept(sfd,NULL,NULL);
                //遍历子进程数，获取空闲子进程
                for(j=0;j<childProNum;j++)
                {
                    if(0==pinfo[i].busy)
                    {
                        //发送new_fd，使子进程去响应客服端的要求
                        send_fd(pinfo[i].fds,new_fd);
                        //设置子进程状态为忙碌
                        pinfo[i].busy=1;
                        printf("%d is busy\n",pinfo[i].pid);
                        break;
                    }
                }
                //关闭父进程对new_fd的引用计数，使父进程不在与客服端有联系
                //当子进程关闭new_fd时能正常关闭
                close(new_fd);
            }
            //如果管道读端是子进程的，说明子进程已经完成任务
            for(j=0;j<childProNum;j++)
            {
                //遍历确定具体的子进程
                if(evs[i].data.fd==pinfo[i].fds)
                {
                    //读取子进程发送的完成任务标志，并将其设置为不忙碌
                    pinfo[i].busy=0;
                    read(pinfo[i].fds,&flag,sizeof(char));
                    printf("%d is not busy\n",pinfo[i].pid);
                    break;
                }
            }
            //如果读端是退出管道,说明服务器要关闭或者升级了，需要有序关闭子进程
            if(exitfds[0]==evs[i].data.fd)
            {
                  //关闭sfd,不接受新的客服端请求
                  close(sfd);
                  //解除epoll监测
                  epoll_ctl(epfd,EPOLL_CTL_DEL,sfd,&event);
                  //通知子进程关闭
                  for(j=0;j<childProNum;j++)
                  {
                      kill(pinfo[j].pid,SIGQUIT);
                  }
                  for(j=0;j<childProNum;j++)
                  {
                      wait(NULL);
                      printf("child exit success\n");
                  }
                  //父进程退出
                  exit(0);
            }
        }
    }
    return 0;
}

