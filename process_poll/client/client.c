#include "cfunc.h"

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
    ret= recv_n(sfd,&dataLen,sizeof(int));
    if(-1==ret)
    {
        close(sfd);
        printf("server crash\n");
        return -1;
    }
    ret= recv_n(sfd,buf,dataLen);
    if(-1==ret)
    {
        close(sfd);
        printf("server crash\n");
        return -1;
    }
    int fd;
    printf("%s\n",buf);
    fd = open(buf,O_RDWR|O_CREAT,0666);//为什么要创建的文件名字处填，因为buf里面填了文件名
    if(-1==fd)
    {
        perror("client open");
        return -1;
    }
    int fileLoadsize=0,filetotalsize=0;//记录当前接收的数据的总长度i
    ret= recv_n(sfd,&dataLen,sizeof(int));
    if(-1==ret)
    {
        close(sfd);
        printf("server crash\n");
        return -1;
    }
    ret= recv_n(sfd,&filetotalsize,dataLen);
    if(-1==ret)
    {
        close(sfd);
        printf("server crash\n");
        return -1;
    }
    int fileslice=filetotalsize/10000;
    int last=0;
    while(1)
    {
        ret= recv_n(sfd,&dataLen,sizeof(int));
        if(-1==ret)
        {
            close(sfd);
            printf("server crash\n");
            return -1;
        }
        if(dataLen>0)
        {
            ret= recv_n(sfd,buf,dataLen);
            if(-1==ret)
            {
                close(sfd);
                printf("server crash\n");
                return -1;
            }
            fileLoadsize+=dataLen;
            // recv(sfd,buf,dataLen,0); 由于接收缓冲区和发送缓冲区大小有限，因此不能直接使用recv函数
            write(fd,buf,dataLen);
            if(fileLoadsize-last>fileslice)
            {
                last=fileLoadsize;
                printf("%5.2f%s\r",(double)fileLoadsize/filetotalsize*100,"%");
                fflush(stdout);
            }
        }else{
            printf("100.00%s\n","%");
            close(sfd);
            printf("recv success\n");
            break;
        }
    }
}
