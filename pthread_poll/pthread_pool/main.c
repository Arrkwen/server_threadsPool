#include "factory.h"
void* threadfunc(void *p)
{
    pFactory pf=(pFactory)p;
    pQue_t pq=&pf->que;
    pNode_t pcur;
    while(1)
    {
        pthread_mutex_lock(&pq->queMutex);
        if(!pq->queSize)
        {
            pthread_cond_wait(&pf->cond,&pq->queMutex);
        }
        queGet(pq,&pcur);
        pthread_mutex_unlock(&pq->queMutex);
        if(NULL!=pcur)
        {

            tranFile(pcur->sfd);
            free(pcur);
        }
        pcur=NULL;
    }
}

int main(int argc,char **argv)
{
    args_check(argc,5);
    Factory f;
    int threadnum=atoi(argv[3]);
    int factoryCapacity=atoi(argv[4]);
    factoryInit(&f,threadnum,factoryCapacity,threadfunc);
    factoryStart(&f);
    int sfd;
    tcpInit(&sfd,argv[1],argv[2]);
    listen(sfd,threadnum);
    int new_fd;
    pQue_t pq=&f.que;
    pNode_t pnew;
    printf("start server:\n");
    while(1)
    {
        new_fd=accept(sfd,NULL,NULL);
        pnew=(pNode_t)calloc(1,sizeof(Node_t));
        pnew->sfd=new_fd;
        pthread_mutex_lock(&pq->queMutex);
        queInsert(pq,pnew);
        pthread_mutex_unlock(&pq->queMutex);
        pthread_cond_signal(&f.cond);
    }
    return 0;
}

