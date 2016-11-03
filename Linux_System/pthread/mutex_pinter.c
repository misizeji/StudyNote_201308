
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
void printer(char *p)
{
	pthread_mutex_lock(&mutex);
	while(*p != '\0')
	{
		sleep(1);
		printf("%c\n",*p);
		p++;

	}
	pthread_mutex_unlock(&mutex);

}

void *func1(void *arg)
{
	char *p = "hello";
	printer(p);
}

void *func2(void *arg)
{
	char *p = "world";
	printer(p);
}

int main(int argc, char *argv[])
{
	pthread_t tid1,tid2;
	pthread_mutex_init(&mutex,NULL);

	pthread_create(&tid1,NULL,func1,NULL);
	pthread_create(&tid2,NULL,func2,NULL);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	pthread_mutex_destroy(&mutex);
	return 0;
}
