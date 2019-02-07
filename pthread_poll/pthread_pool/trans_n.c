#include "transfile.h"

int send_n(int sfd,void *paddr,int len)
{
     int total=0;
     int ret;
     while(total<len)
     {
         ret=send(sfd,(char*)paddr+total,len-total,0);
         //如果客服端断开，发送失败，返回-1
         if(-1==ret)
         {
             return -1;
         }
         total+=ret;
     }
     return 0;
}

int recv_n(int new_fd,void *paddr,int len)
{
     int total=0;
     int ret;
     while(total<len)
     {
         ret=recv(new_fd,(char*)paddr+total,len-total,0);
         //如果服务器断开，接收数据为0，需要要断开连接
         if(0==ret)
         {
             return -1;
         }
         total+=ret;
     }
     return 0;
}
