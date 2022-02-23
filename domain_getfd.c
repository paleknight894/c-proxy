#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include"sockstypes.h"
unsigned int domain_getfd(struct request_t *request)
{
	struct addrinfo hints,*res,*p=NULL;
	unsigned int fd,len=request->address[0];
	uint16_t port;
	char * str=(char *)malloc(len+1), *str_port=(char *)malloc(6);
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	memcpy((void *)&port,(void *)&(request->address[1+len]),2);
	memcpy((void *)str,(void*)&(request->address[1]),len);
	sprintf(str_port,"%d",ntohs(port));
	if(getaddrinfo(str,str_port,&hints,&res)!=0)
	{
		free(str);
		free(str_port);
		freeaddrinfo(res);
		return 0;
	}
	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	for(p=res;p!=NULL;p=p->ai_next)
	{
		if(connect(fd,res->ai_addr,res->ai_addrlen)==-1)
			continue;
		break;
	}
	if(p==NULL)
	{
		close(fd);
		fd=0;
		perror("domain connect");
	}
	free(str);
	free(str_port);
	freeaddrinfo(res);
	return fd;
}
