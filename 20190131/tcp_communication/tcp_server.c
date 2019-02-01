#include <func.h>

int main(int argc,char **argv)
{
    args_check(argc,3);
    struct sockaddr_in my_sockaddr;
    memset(&my_sockaddr,0,sizeof(struct sockaddr_in));
    my_sockaddr.sin_family=AF_INET;
    my_sockaddr.sin_addr.s_addr=inet_addr(argv[1]);
    my_sockaddr.sin_port=htons(atoi(argv[2]));
    int sfd = socket(AF_INET,SOCK_STREAM,0);//套接字描述符(sfd,new_fd)拥有两个缓冲区，发送和接收缓冲区
    //sfd缓冲区用于存储本次通信的ip,端口等信息，即结构体sockaddr_in，new_fd用于存储通信的内容
    if(-1==sfd)
    {
        printf("socket");
        return -1;
    }
   // printf("the port=%x,the ip=%x\n",my_sockaddr.sin_port,my_sockaddr.sin_addr.s_addr);
    int ret= bind(sfd,(struct sockaddr*)&my_sockaddr,sizeof(struct sockaddr_in));
    if(-1==ret)
    {
        printf("bind");
        return -1;
    }
    listen(sfd,10);
    struct sockaddr_in client;
    memset(&client,0,sizeof(struct sockaddr_in));
    socklen_t addrlen=sizeof(struct sockaddr_in);
    int new_fd=accept(sfd,(struct sockaddr*)&my_sockaddr,&addrlen);
    if(-1==new_fd)
    {
        printf("accpt");
        return -1;
    }
    char recevbuf[128]={0};
    memset(recevbuf,0,sizeof(recevbuf));
    ret = recv(new_fd,recevbuf,sizeof(recevbuf),0);
    if(-1==ret)
    {
        printf("recv");
        return -1;
    }
    printf("server recv from client:%s\n",recevbuf);
    char sendbuf[128]="I git it,thanks";
    ret = send(new_fd,sendbuf,strlen(sendbuf),0);
    close(new_fd);//关闭通信缓冲区
    close(sfd);//关闭监听窗口，结束会话
    return 0;
}

