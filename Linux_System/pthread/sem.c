
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;
void printer(char *p)
{
	sem_wait(&sem);
	while(*p != '\0')
	{
		sleep(1);
		printf("%c\n",*p);
		p++;
	}
	sem_post(&sem);
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
	sem_init(&sem,0,1);
	pthread_create(&tid1,NULL,func1,NULL);
	pthread_create(&tid2,NULL,func2,NULL);
	
	pthread_join(tid1,NULL);
	pthread_join(tid1,NULL);

	sem_destroy(&sem);
	return 0;
}
