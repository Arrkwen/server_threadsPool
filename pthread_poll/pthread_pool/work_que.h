#ifndef __WORK_H__
#define __WORK_H__

#include "head.h"
//队列节点结构体
typedef struct node{
    int sfd;
    struct node *next;
}Node_t,*pNode_t;

//队列
typedef struct{
    pNode_t queHead,queTail;
    //队列容量（即所允许的最大连接量
    int queCapacity;
    //实际容量
    int queSize;
    //线程锁，互斥访问队列
    pthread_mutex_t queMutex;
}Que_t,*pQue_t;

void queInsert(pQue_t,pNode_t);
void queGet(pQue_t,pNode_t*);
#endif
