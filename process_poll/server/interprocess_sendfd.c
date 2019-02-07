#include "pfunc.h"
int send_fd(int socketpairw,int fd)
{
    char buf[20]="hello world";
   //初始化消息结构体里的内容结构体
    struct iovec msg_iov;//消息内容结构体
    msg_iov.iov_base=buf;//缓冲区起始地址
    msg_iov.iov_len=strlen(buf);//缓冲区数据长度
    //初始化消息结构体里面的控制结构体
    struct cmsghdr *cmsg;
    int len=CMSG_LEN(sizeof(int));//计算所传信息长度
    cmsg=(struct cmsghdr*)calloc(1,len);//申请结构体空间并初始化为0
    cmsg->cmsg_len=len;//本结构体长度，为了确定消息结构体中的数据起始地址
    cmsg->cmsg_level=SOL_SOCKET;//消息传送协议
    cmsg->cmsg_type=SCM_RIGHTS;
    *(int*)CMSG_DATA(cmsg)=fd;//CMSG获取cmsg的长度，确定fd存放的的地址，地址先强转，再解引用赋值
    //初始化消息结构体   
    struct msghdr msg;//消息结构体，利用前两者进行赋值
    memset(&msg,0,sizeof(msg));
    msg.msg_iov=&msg_iov;//缓冲区的起始地址
    msg.msg_iovlen=1;//缓冲区的个数
    msg.msg_control=cmsg;
    msg.msg_controllen=len;
    int ret=sendmsg(socketpairw,&msg,0);//发送消息，将msg结构体的内容发写道socketpairw管道的一端
    if(-1==ret)
    {
        perror("sendmsg");
        return -1;
    }
}

int recv_fd(int socketpairr,int *fd)//接收socketpair管道里的数据
{
    char buf[20]={0};//申请接收消息的空间大小
   //初始化消息结构体里的内容结构体
    struct iovec msg_iov;//消息内容结构体
    msg_iov.iov_base=buf;//消息起始地址
    msg_iov.iov_len=11;//消息长度
    //初始化消息结构体里面的控制结构体
    struct cmsghdr *cmsg;
    int len=CMSG_LEN(sizeof(int));
    cmsg=(struct cmsghdr*)calloc(1,len);//申请结构体空间并初始化为0
    cmsg->cmsg_len=len;//本结构体长度，为了确定消息结构体中的数据起始地址
    cmsg->cmsg_level=SOL_SOCKET;//消息传送协议
    cmsg->cmsg_type=SCM_RIGHTS;
    //初始化消息结构体   
    struct msghdr msg;//消息结构体，利用前两者进行赋值
    memset(&msg,0,sizeof(msg));
    msg.msg_iov=&msg_iov;
    msg.msg_iovlen=1;
    msg.msg_control=cmsg;
    msg.msg_controllen=len;
    int ret=recvmsg(socketpairr,&msg,0);//接收消息，将socketpairw管道的内容写道消息结构体里指定的buf中
    if(-1==ret)
    {
        perror("recvmsg");
        return -1;
    }
    *fd= *(int*)CMSG_DATA(cmsg);//将消息的描述符号赋值给fd
}

