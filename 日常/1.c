#include <stdio.h>
#include "my_socket.h"

int main()
{
	struct sockaddr_in cli;
	int sock_fd = my_accept_seve();
	int len;
int ret = accept(sock_fd, (struct sockaddr*)&cli, &len);
	printf("连接成功%d\n",ret);
	while(1)
	{
	}
}
