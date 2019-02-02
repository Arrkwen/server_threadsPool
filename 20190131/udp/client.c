
#include <func.h>

int main(int argc,char **argv)
{
    args_check(argc,3);
    struct sockaddr_in my_sockaddr;
    memset(&my_sockaddr,0,sizeof(struct sockaddr_in));
    my_sockaddr.sin_family=AF_INET;
    my_sockaddr.sin_addr.s_addr=inet_addr(argv[1]);
    my_sockaddr.sin_port=htons(atoi(argv[2]));
    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    if(-1==sfd)
    {
        printf("socket");
        return -1;
    }
    socklen_t addrlen=sizeof(my_sockaddr);
    int ret; 
    char buf[128]={0};
    fd_set rdset;
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(sfd,&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        ret = select(sfd+1,&rdset,NULL,NULL,NULL);
        if(ret>0)
        {
            if(FD_ISSET(STDIN_FILENO,&rdset))
            {
                memset(buf,0,sizeof(buf));
                ret=read(STDIN_FILENO,buf,sizeof(buf)-1);
                if(-1==sfd)
                {
                    printf("read");
                    return -1;
                }
                sendto(sfd,buf,strlen(buf)-1,0,(struct sockaddr*)&my_sockaddr,sizeof(struct sockaddr_in));
            }
            if(FD_ISSET(sfd,&rdset))
            {
                memset(buf,0,sizeof(buf));
                addrlen=sizeof(struct sockaddr);      
                ret= recvfrom(sfd,buf,sizeof(buf)-1,0,(struct sockaddr*)&my_sockaddr,&addrlen);
                if(0==ret)
                {
                    close(sfd);
                    break;
                }
                printf("%s",buf);
            }
        }
    }
}
