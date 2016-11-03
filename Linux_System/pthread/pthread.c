
#include <stdio.h>
#include <pthread.h>


void *func1 (void *arg)
{
	while(1)
	{

		printf("in func1\n");
		sleep(1);
	}
}

void *func2 (void *arg)
{
	int in = 0;
	in = *((int *)arg);
	while(1)
	{
		printf("in func2 the num is %d\n",in);
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	int num =123;
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,func1,NULL);
	pthread_create(&tid2,NULL,func2,(void *)&num);
	while(1);
	return 0;
}
