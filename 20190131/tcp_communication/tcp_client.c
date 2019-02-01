
#include <func.h>

int main(int argc,char **argv)
{
    args_check(argc,3);
    struct sockaddr_in my_sockaddr;
    memset(&my_sockaddr,0,sizeof(struct sockaddr_in));
    my_sockaddr.sin_family=AF_INET;
    my_sockaddr.sin_addr.s_addr=inet_addr(argv[1]);
    my_sockaddr.sin_port=htons(atoi(argv[2]));
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1==sfd)
    {
        printf("socket");
        return -1;
    }
    socklen_t addrlen=sizeof(my_sockaddr);
    int ret=connect(sfd,(struct sockaddr*)&my_sockaddr,addrlen);
    if(-1==ret)
    {
        printf("connect");
        return -1;
    }
    char recvbuf[128]={0};
    char sendbuf[128]="hello,give you a present";
    send(sfd,sendbuf,strlen(sendbuf),0);
    recv(sfd,recvbuf,sizeof(recvbuf),0);
    printf("client recv from server:%s\n",recvbuf);
    close(sfd);
}
