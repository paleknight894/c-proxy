#include<netdb.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include"sockstypes.h"
unsigned int ipv6_getfd(struct request_t *request)
{
    struct sockaddr_in6 *proxyaddr=(struct sockaddr_in6*)malloc(sizeof(struct sockaddr_in6));
    unsigned int fd;
    fd=socket(PF_INET6,SOCK_STREAM,0);
    proxyaddr->sin6_family=AF_INET6;
    memcpy((void *)(proxyaddr->sin6_addr.s6_addr),(void*)(request->address),16);
    memcpy((void *)&(proxyaddr->sin6_port),(void*)&(request->address[16]),2);
    if(connect(fd,(struct sockaddr *)proxyaddr,sizeof(struct sockaddr_in6))==-1)
	{
		close(fd);
		fd=0;
		perror("ipv6 connect:");
	}
    free(proxyaddr);
    return fd;
}
