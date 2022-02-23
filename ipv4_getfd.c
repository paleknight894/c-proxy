#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"sockstypes.h"
unsigned int ipv4_getfd(struct request_t *request)
{
	struct sockaddr_in *proxyaddr=(struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
	unsigned int fd;
	fd=socket(PF_INET,SOCK_STREAM,0);
	proxyaddr->sin_family=AF_INET;
	memcpy((void *)&(proxyaddr->sin_addr),(void*)(request->address),4);
	memcpy((void *)&(proxyaddr->sin_port),(void*)&(request->address[4]),2);
	if(connect(fd,(struct sockaddr *)proxyaddr,sizeof(struct sockaddr))==-1)
	{
		close(fd);
		fd=0;
		perror("ipv4 connect:");
	}
	free(proxyaddr);
	return fd;
}
