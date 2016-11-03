
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	int ret = 0;
	ret = alarm(5);
	printf("the time left %d\n",ret);
	sleep(2);
	ret = alarm(1);
	printf("the time left %d\n",ret);
	while(1);
	return 0;
}
