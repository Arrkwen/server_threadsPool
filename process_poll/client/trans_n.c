#include "cfunc.h"

void send_n(int sfd,void *paddr,int len)
{
     int total=0;
     int ret;
     while(total<len)
     {
         ret=send(sfd,(char*)paddr+total,len-total,0);
         total+=ret;
     }
     return;
}

void recv_n(int new_fd,void *paddr,int len)
{
     int total=0;
     int ret;
     while(total<len)
     {
         ret=recv(new_fd,(char*)paddr+total,len-total,0);
         total+=ret;
     }
     return;
}
