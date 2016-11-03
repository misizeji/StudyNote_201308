/* ************************************************************************
 *       Filename:  bma150_test.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2012年11月17日 21时19分23秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct _position {
	signed short x;
	signed short y;
	signed short z;
}position_t;

int main(void)
{
	int fd = -1, ret ;
	position_t	position;
	fd = open("/dev/bma_150", O_RDWR);
	if (fd < 0) {
		perror("open");
	}
	sleep(1);
	//ioctl(); //CTRL_SET
	while(1) {
		ret = read(fd, &position, sizeof(position_t));
		if (ret == -1) 
			break;
		printf("\tx=%d\ty=%d\tz=%d\n", position.x, position.y, position.z);
		usleep(1000*100);
	}
	return 0;

}


