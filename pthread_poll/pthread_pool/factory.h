#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "work_que.h"
#include "transfile.h"

//定义函数指针
typedef void*(*threadfunc_t)(void*);

//定义协议结构体
typedef struct{
    //定义线程id,由于不确定个数，故定义为指针类型
    pthread_t *pthid;
    int pthreadNum;
    //客服端申请队列
    Que_t que;
    //唤醒线程的信号
    pthread_cond_t cond;
    //线程处理函数
    threadfunc_t DownFilefunc;
    //标志位
    short startflag;
}Factory,*pFactory;

void factoryInit(pFactory,int,int,threadfunc_t);
void factoryStart(pFactory);
int tcpInit(int *,char*,char*);
#endif



