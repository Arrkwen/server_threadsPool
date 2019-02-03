#include "cfunc.h"
//说明，为什么不能像上一次一样直接调用send
//1因为服务器发送缓冲区和接收缓冲区大小有限
//2send函数只是将数据从buf里面送到sfd的发送缓冲区
//3由于接收双方的速度不匹配，缓冲区残留数据，导致发送方往缓冲区一次性应放1004个字节的，
//却只能放少于1004个字节的数据，其他数据要等待下一次缓冲区清空才能往里放
//故导致接收方在接收数据时，本来头部长度是1004字节，但是实际数据缺不足，从而发生接收错误
//因此要使用循环发送和循环读取
void send_n(int sfd,void *paddr,int len)
{
     int total=0;
     int ret;
     while(total<len)//当发送数据小于应发数据长度时
     {
         ret=send(sfd,(char*)paddr+total,len-total,0);//第二个参数：数据在包中的地址要发生偏移，第三个参数也要改变
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
