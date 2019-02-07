#include "work_que.h"

void queInsert(pQue_t pque,pNode_t pnew)
{
    if(pque->queHead==NULL)
    {
        pque->queHead=pnew;
        pque->queTail=pnew;
    }else{
        pque->queTail->next=pnew;
        pque->queTail=pnew;
}

        pque->queSize++;
}

void queGet(pQue_t pq,pNode_t* pget)
{
      if(!pq->queSize)
      {
          return;
      }else{
          *pget=pq->queHead;
          pq->queHead=pq->queHead->next;
          pq->queSize--;
      }
}
