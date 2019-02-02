#include "pfunc.h"

int main(int argc ,char **argv)
{
    int i,j;
    int sfd;
    int new_fd;
    int readyset;
    char flag;
    args_check(argc,4);
    int childProNum=atoi(argv[3]);
    proInfo *pinfo=(proInfo*)calloc(childProNum,sizeof(proInfo));
    createChildProcess(pinfo,childProNum);
    tcpInit(&sfd,argv[1],argv[2]);
    listen(sfd,childProNum);
    int epfd=epoll_create(1);
    struct epoll_event event,*evs;
    evs=(struct epoll_event*)calloc(childProNum+1,sizeof(struct epoll_event));
    event.events=EPOLLIN;
    event.data.fd=sfd;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
    if(-1==ret)
    {
        perror("epoll_ctl");
        return -1;
    }
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
    printf("start server:\n");
    while(1)
    {
        memset(evs,0,sizeof(struct epoll_event));
        readyset=epoll_wait(epfd,evs,childProNum+1,-1);
        for(i=0;i<readyset;i++)
        {
            if(evs[i].data.fd==sfd)
            {
                new_fd=accept(sfd,NULL,NULL);
                for(j=0;j<childProNum;j++)
                {
                    if(0==pinfo[i].busy)
                    {
                        send_fd(pinfo[i].fds,new_fd);
                        close(new_fd);
                        pinfo[i].busy=1;
                        printf("%d is busy\n",pinfo[i].pid);
                        break;
                    }
                }
            }
            for(j=0;j<childProNum;j++)
            {
                if(evs[i].data.fd==pinfo[i].fds)
                {
                    pinfo[i].busy=0;
                    read(pinfo[i].fds,&flag,sizeof(char));
                    printf("%d is not busy\n",pinfo[i].pid);
                    break;
                }
            }
        }
    }
    close(sfd);
    return 0;
}

