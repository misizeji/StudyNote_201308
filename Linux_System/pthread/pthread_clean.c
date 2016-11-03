
#include <stdio.h>
#include <pthread.h>
void cleanup_func(void* arg)
{
	printf("in cleanup_func\n");
}

void *func(void *arg)
{
	
	pthread_cleanup_push(cleanup_func,NULL);
	sleep(2);
	pthread_cleanup_pop(1);
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
