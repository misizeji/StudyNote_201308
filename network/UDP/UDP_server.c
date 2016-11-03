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
	int err_log = 0;
	unsigned short port = 8000;
	struct sockaddr_in my_addr;
	if (argc > 1)
	{	
		port = atoi (argv[1]);
	}
	
	printf("UDP Server Started!\n");
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	printf("Binding server to port %d\n",port);
	
	
	err_log = bind (sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	printf("waiting data from other client...\n");
	while(1)
	{
		char recv_buf[512] = "";
		char cli_ip[INET_ADDRSTRLEN] = "";
		int recv_len = 0;
		struct sockaddr_in client_addr;
		socklen_t cliaddr_len = sizeof (client_addr);
		
		recv_len = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0,\
		(struct sockaddr*)&client_addr, &cliaddr_len);
		printf("client ip = %s\n",\
		inet_ntop(AF_INET,&client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN));
		sendto(sockfd, recv_buf,recv_len, 0, \
		(struct sockaddr*)&client_addr, cliaddr_len);
		
	}
	
	close(sockfd);
	return 0;
 }