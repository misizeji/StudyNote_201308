#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>


int main(int argc,char *argv[])
{
	unsigned long key1_5 = 0;
	//unsigned long key1_5_before = 0;
	int key_fd = open("/dev/key_driver",O_RDWR);
	
	while(1)
	{
		read(key_fd,&key1_5,sizeof(key1_5));
		//if((key1_5 != 0) && (key1_5 != key1_5_before))
		if(key1_5 != 0)
		{
			printf("you press the key%d\n",key1_5);
			//key1_5_before = key1_5;
		}
		else
		{
			;
		}
		usleep(200*1000);
	}
	close(key_fd);
	return 0;
}