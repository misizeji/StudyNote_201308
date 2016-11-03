#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

struct termios options, oldoptions;

/****************************************
�������ܣ����ڳ�ʼ��
�������ͣ�
�������ͣ����ش򿪴��ڵ��ļ�������
*****************************************/
int Uart_Init()
{
	int fd = -1;
	fd = open("/dev/s3c2410_serial2", O_RDWR);
	tcgetattr(fd, &oldoptions);
	tcgetattr(fd, &options);
	
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|IGNCR|ICRNL|IXON);
	
	options.c_cflag &= ~PARENB; //����żУ��λ
	options.c_cflag &= ~CSTOPB;	//ֹͣλΪ1λ
	options.c_cflag &= ~CSIZE;	
	options.c_cflag |= CS8;		//����λΪ8λ
	
	options.c_lflag &= ~(ICANON|ECHO|ECHOE|ISIG);
	tcsetattr(fd,TCSANOW,&options);
	
	return fd;
}

/****************************************
�������ܣ�GPRSģ��ĳ�ʼ��
�������ͣ������ļ�������
�������ͣ�
*****************************************/
void GPRS_init(int fd_uart)
{
	write(fd_uart, "AT+CMIC=0,15\n",	strlen("AT+CMIC=0,15\n"));//����MIC
	write(fd_uart, "AT+CHFA=1\n",		strlen("AT+CHFA=1\n"));//������Ƶͨ��
	write(fd_uart, "AT+CLVL=100\n",		strlen("AT+CLVL=100\n"));//���ö�������
	write(fd_uart, "AT+CPIL=1\n",		strlen("AT+CPIL=1\n"));//����������ʾ
	
	write(fd_uart, "AT+CMGF=1\n",		strlen("AT+CMGF=1\n"));//���÷��Ͷ��Ÿ�ʽ
	write(fd_uart, "AT+CSCS=\"GSM\"\n",	strlen("AT+CSCS=\"GSM\"\n"));//���ö��ű����ʽ
}

int gprs_deal(char *phone_num, char *phone_msg)
{
	int fd_uart = Uart_Init();	//���ڳ�ʼ�������򿪴����豸�ļ�
	char send_num[100] = {0};
	char send_msg[100]= {0};
	printf("fd_uart=%d\n", fd_uart);

	GPRS_init(fd_uart);//
	usleep(200*1000);

	//call somebody
	//write(fd_uart, "ATD18612491570;\n",	strlen("ATD18612491570;\n"));
	//write(fd_uart, "ATD112;\n",	strlen("ATD112;\n"));

	//send msg to somebody
	//char *phone_num = "18003614582";
	sprintf(send_num,"AT+CMGS=\"%s\"\n",phone_num);
	int ret = write(fd_uart, send_num,strlen(send_num));
	printf("_____%d______\n",ret);
	
	usleep(100*1000);
	//char *phone_msg = "aaaaaa";
	sprintf(send_msg,"%s\032\n",phone_msg);
	ret = write(fd_uart,send_msg,strlen(send_msg));
	printf("_____%d______\n",ret);

	printf("===over===\n");
	close(fd_uart);
	return 0;
}






