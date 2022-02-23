#include<stdint.h>
#include<netdb.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include <arpa/inet.h>
#include"sockstypes.h"

void tcp_forward(int,int);
void sigchld_handler(int s)
{
	if(s==SIGCHLD)
		while(waitpid(-1,NULL,WNOHANG)>0);
}
int method_exists(struct client_hello_t * client_hello,uint8_t method);
unsigned int ipv4_getfd(struct request_t *request);
unsigned int ipv6_getfd(struct request_t *request);
unsigned int domain_getfd(struct request_t *request);
int main(int argc,char *argv[])
{
	int i;
	unsigned int sockfd,new_fd;
	struct addrinfo hints,*res;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	pid_t pid;
	struct sigaction sa;
	sa.sa_handler=sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_RESTART;
	sigaction(SIGCHLD,&sa,NULL);
	addr_size=sizeof(their_addr);
	if(argc<3)
	{
		fprintf(stderr,"usege : %s listen-address listen-port\n",argv[0]);
		exit(-1);
	}
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	i=getaddrinfo(argv[1],argv[2],&hints,&res);
	if(i!=0)
	{
		fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(i));
		exit(-1);
	}
	if((sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol))==-1)
	{
		perror("get sockfd error\n");
		exit(-1);
	}
	if(bind(sockfd,res->ai_addr,res->ai_addrlen))
	{
		perror("bind error\n");
		exit(-1);
	}
	freeaddrinfo(res);
	if((listen(sockfd,5)))
	{
		perror("listen error\n");
		exit(-1);
	}
	while(1)
	{
		new_fd=accept(sockfd,(struct sockaddr *)&their_addr,&addr_size);
		pid=fork();
		if(!pid)
		{
			close(sockfd);
			unsigned int proxyfd;
			struct client_hello_t *client_hello=(struct client_hello_t *)malloc(sizeof(struct client_hello_t));
			struct server_hello_t *server_hello=(struct server_hello_t *)malloc(sizeof(struct server_hello_t));
			struct request_t *request=(struct request_t *)malloc(sizeof(struct request_t));
			struct reply_t *reply=(struct reply_t *)malloc(sizeof(struct reply_t));
			recv(new_fd,client_hello,sizeof(struct client_hello_t),0);
			server_hello->ver=0x05;
			if(method_exists(client_hello,0x00))
			{
				server_hello->method=0x00;
			}
			else
			{
				server_hello->method=0xff;
			}
			send(new_fd,server_hello,sizeof(struct server_hello_t),0);
			recv(new_fd,request,sizeof(struct request_t),0);
			if (request->cmd != 0x01) 
			{
				reply->rep = 0x07;
			}
			reply->ver=0x05;
			reply->atyp=0x01;
			if(request->atyp==0x01)
			{
				proxyfd=ipv4_getfd(request);
				if(proxyfd>0)
				{
					reply->rep=0x00;
				}
				else
				{
					reply->rep=0x01;
				}
			}else if(request->atyp==0x03)
			{
				proxyfd=domain_getfd(request);
				if(proxyfd>0)
				{
					reply->rep=0x00;
				}
				else
				{
					reply->rep=0x01;
				}
			}else if(request->atyp==0x04)
			{
                proxyfd=ipv6_getfd(request);
                if(proxyfd>0)
                {
                    reply->rep=0x00;
                }
                else
                {
                    reply->rep=0x01;
                }
			}
			send(new_fd,reply,10,0);
			if(reply->rep==0x00)
			{
				tcp_forward(new_fd,proxyfd);
				close(proxyfd);
			}
			free(request);
			free(reply);
			free(client_hello);
			free(server_hello);
			close(new_fd);
			return 0;
		}
		close(new_fd);
	}
	return 0;
}
