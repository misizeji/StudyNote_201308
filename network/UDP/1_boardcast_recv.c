 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>
 #include <netinet/in.h>
 #include <sys/socket.h>
 #include <arpa/inet.h>
 
 int main(int argc, char *argv[])
 {
	int sockfd = 0;					//socket
	int err_log = 0;				//ret of bind
	unsigned short port = 8000;		//port :8000
	struct sockaddr_in my_addr;
	if (argc > 1)
	{	
		port = atoi (argv[1]);
	}
	
	printf("UDP Server Started!\n");
	sockfd = socket(AF_INET,SOCK_DGRAM,0);	//use the UDP
	if (sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	bzero(&my_addr, sizeof(my_addr));		//clean bzero
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	err_log = bind (sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));  //bind
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	printf("waiting data from boardcast...\n");
	while(1)
	{
		char recv_buf[512] = "";			//receive the msg
		//int recv_len = 0;					//length of receive
		struct sockaddr_in client_addr;
		socklen_t cliaddr_len = sizeof (client_addr);
		
		recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0,(struct sockaddr*)&client_addr, &cliaddr_len);	
		printf("%s\n",recv_buf);
	}
	
	close(sockfd);		//shutdown the socket
	return 0;
 }