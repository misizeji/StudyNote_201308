#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int sockfd = 0;
	struct sockaddr_in dest_addr;
	unsigned short port = 8000;
	char *server_ip = "10.220.2.189";

	if (argc > 1)
	{
		server_ip = argv[1];
	}
	if (argc > 2)
	{
		port = atoi(argv[2]);
	}

	sockfd = socket (AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}

	bzero(&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	inet_pton(AF_INET, server_ip, &dest_addr.sin_addr);

	printf("ready send data to UDP server %s:%d\n", server_ip, port);
	while(1)
	{
		char send_buf[512] = "";
		char recv_buf[512] = "";

		fgets(send_buf, sizeof(send_buf), stdin);
		send_buf[strlen(send_buf) - 1] = '\0';
		
		sendto(sockfd, send_buf, strlen(send_buf), 0, \
				(struct sockaddr*)&dest_addr, sizeof(dest_addr));

		recvfrom(sockfd,recv_buf,sizeof(recv_buf), 0,NULL,NULL);
		printf("%s\n",recv_buf);
	}

	close(sockfd);
	return 0;
}
