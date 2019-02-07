#include "factory.h"

void factoryInit(pFactory pf,int threadNum,int capacity,threadfunc_t threadfunc)
{
    memset(pf,0,sizeof(Factory));
    pf->pthreadNum=threadNum;
    pf->que.queCapacity=capacity;
    pf->DownFilefunc=threadfunc;
    pf->pthid=(pthread_t*)calloc(threadNum,sizeof(pthread_t));
    pthread_mutex_init(&pf->que.queMutex,NULL);
    pthread_cond_init(&pf->cond,NULL);
}

void factoryStart(pFactory pf)
{
     int i;
     if(0==pf->startflag)
     {
         for(i=0;i<pf->pthreadNum;i++)
         {
             pthread_create(pf->pthid+i,NULL,pf->DownFilefunc,pf);
         }
         pf->startflag=1;
     }
}
