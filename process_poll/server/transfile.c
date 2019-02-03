#include "pfunc.h"

int transFile(int new_fd)
{
    //size_t filetotalsize,fileloadsize;
    transDatagram d;
    //传送文件名：
    strcpy(d.buf,Download);
    d.dataLen=strlen(Download);
    int ret=send(new_fd,&d,4+d.dataLen,0);
    printf("sever send ret=%d\n",ret);
    int fd=open(Download,O_RDONLY);
    if(-1==fd)
    {
        perror("open");
        return -1;
    }
    //传送文件内容
    while(d.dataLen=read(fd,d.buf,sizeof(d.buf)))
    {
        send(new_fd,&d,4+d.dataLen,0);
    }
    send(new_fd,&d,4+d.dataLen,0);//发送结束标志，因为发送结束之后，d.datalen==0
    return 0;
}

