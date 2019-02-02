#include "pfunc.h"

int tcpInit(int *sfd,char *ip,char *port)
{
    struct sockaddr_in my_sockaddr;
    memset(&my_sockaddr,0,sizeof(struct sockaddr_in));
    my_sockaddr.sin_family=AF_INET;
    my_sockaddr.sin_addr.s_addr=inet_addr(ip);
    my_sockaddr.sin_port=htons(atoi(port));
    *sfd=socket(AF_INET,SOCK_STREAM,0);//套接字描述符(sfd,new_fd)拥有两个缓冲区，发送和接收缓冲区
    //sfd缓冲区用于存储本次通信的ip,端口等信息，即结构体sockaddr_in，new_fd用于存储通信的内容
    if(-1==*sfd)
    {
        printf("socket");
        return -1;
    }
    //setsockopt 设置sfd的属性，端口可重用
    int reuse = 1;
    int ret=setsockopt(*sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    if(-1==ret)
    {
        printf("setsockopt");
        return -1;
    }
    // printf("the port=%x,the ip=%x\n",my_sockaddr.sin_port,my_sockaddr.sin_addr.s_addr);
    // 绑定服务器的ip和port
    ret= bind(*sfd,(struct sockaddr*)&my_sockaddr,sizeof(struct sockaddr_in));
    if(-1==ret)
    {
        printf("bind");
        return -1;
    }
    return 0;
}
