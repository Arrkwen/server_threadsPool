#include "pfunc.h"

int transFile(int new_fd)
{
    //size_t filetotalsize,fileloadsize;
    transDatagram d;
    //传送文件名：
    strcpy(d.buf,Download);
    d.dataLen=strlen(Download);
    send_n(new_fd,&d,4+d.dataLen);
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
    send_n(new_fd,&d,4+d.dataLen);
    //传送文件内容
    while(d.dataLen=read(fd,d.buf,sizeof(d.buf)))
    {
        send_n(new_fd,&d,4+d.dataLen);
    }
    send_n(new_fd,&d,4+d.dataLen);//发送结束标志，因为发送结束之后，d.datalen==0
    return 0;
}

