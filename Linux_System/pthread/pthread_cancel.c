
#include <stdio.h>
#include <pthread.h>
void *func(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	int i = 0;
	for (i = 0;i < 5;i++)
	{
		printf("hello world\n");
		sleep(1);
	}
}
int main(int argc, char *argv[])
{
	pthread_t tid;
	pthread_create(&tid,NULL,func,NULL);

	sleep(2);

	pthread_cancel(tid);

	pthread_join(tid,NULL);

	return 0;
}
