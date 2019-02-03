
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
    char buf[1000]={0};
    int dataLen;
    recv(sfd,&dataLen,sizeof(int),0);
    recv(sfd,buf,dataLen,0);
    int fd;
    printf("%s\n",buf);
    fd = open(buf,O_RDWR|O_CREAT,0666);//为什么要创建的文件名字处填，因为buf里面填了文件名
    if(-1==fd)
    {
        perror("client open");
        return -1;
    }
    while(1)
    {
        recv(sfd,&dataLen,sizeof(int),0);
        if(dataLen>0)
        {
            recv(sfd,buf,dataLen,0);
            write(fd,buf,dataLen);
        }else{
            close(sfd);
            printf("recv success\n");
            break;
        }
    }
}
