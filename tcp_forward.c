#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void tcp_forward(int fd1, int fd2)
{
    uint8_t *buf=(uint8_t *) malloc(BUF_SIZE);

    int nfds;
    nfds = fd1 > fd2 ? fd1 : fd2;
    nfds += 1;

    fd_set readfds;
    struct timeval timeout;

    while (1) 
	{
        FD_ZERO(&readfds);
        FD_SET(fd1, &readfds);
        FD_SET(fd2, &readfds);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ret;
        ret = select(nfds, &readfds, NULL, NULL, &timeout);
        if (ret == 0) continue;
        if (ret == -1) {
            perror("select error");
            return;
        }

//        bzero(buf, BUF_SIZE);
        if (FD_ISSET(fd1, &readfds)) 
		{
            ret = recv(fd1, buf, BUF_SIZE, 0);
            if (ret == 0) {
//                printf("Connection closed by fd1 foreign host.\n");
				free(buf);
                return;
            }
            if (ret == -1) {
                perror("recv error");
				free(buf);
                return;
            }
            send(fd2, buf, ret, 0);
        } else if(FD_ISSET(fd2, &readfds)) 
		{
            ret = recv(fd2, buf, BUF_SIZE, 0);
            if (ret == 0) {
//                printf("Connection closed by fd2 foreign host.\n");
				free(buf);
                return;
            }
            if (ret == -1) {
                perror("recv error");
				free(buf);
                return;
            }
            send(fd1, buf, ret, 0);
        }
    }
}
