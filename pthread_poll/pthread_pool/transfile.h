#ifndef __TRANFILE_H__
#define __TRANFILE_H__

#include "head.h"

typedef struct {
    int dataLen;
    char buf[1000];
}train;

#define Download "file"
int tranFile(int new_fd);
int send_n(int sfd,void *ptrain,int len);
int recv_n(int sfd,void *ptrain,int len);
#endif
