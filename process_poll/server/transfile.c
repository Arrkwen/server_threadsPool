#include "pfunc.h"

int transFile(int new_fd)
{
    //size_t filetotalsize,fileloadsize;
    transDatagram d;
    //传送文件名：
    strcpy(d.buf,Download);
    d.dataLen=strlen(Download);
    int ret=send_n(new_fd,&d,4+d.dataLen);
    //返回值为-1，代表客服端断开
    if(-1==ret)
    {
        printf("client close\n");
        close(new_fd);
        return -1;
    }
    //打开文件，获得文件描述符 
    int fd=open(Download,O_RDONLY);
    if(-1==fd)
    {
        perror("open");
        return -1;
    }
    //发送文件大小
    //获取文件大小
    struct stat filesize;
    fstat(fd,&filesize);
    //存入缓冲区   
    d.dataLen=sizeof(filesize.st_size);
    memcpy(d.buf,&filesize.st_size,d.dataLen);
    //发送文件大小
    ret= send_n(new_fd,&d,4+d.dataLen);
    if(-1==ret)
    {
        printf("client close\n");
        close(new_fd);
        return -1;
    }
    //传送文件内容
    while(d.dataLen=read(fd,d.buf,sizeof(d.buf)))
    {
        ret=send_n(new_fd,&d,4+d.dataLen);
        if(-1==ret)
        {
            printf("client close\n");
            close(new_fd);
            return -1;
        }
    }
    ret=send_n(new_fd,&d,4+d.dataLen);//发送结束标志，因为发送结束之后，d.datalen==0
    if(-1==ret)
    {
        printf("client close\n");
        close(new_fd);
        return -1;
    }
    return 0;
}

