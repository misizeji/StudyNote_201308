#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

char ch = 'a';
sem_t sem1,sem2;	
void *func1(void *arg)
{
	while(1)
	{
		sem_wait(&sem1);
		ch = ch + 1;
		printf("the ansic is %d\n",ch);
		sleep(1);
		sem_post(&sem2);
	}
}

void *func2(void *arg)
{
	while(1)
	{
		sem_wait(&sem2);
		printf("the character is %c\n",ch);
		sleep(1);
		sem_post(&sem1);
	}
}


int main(int argc, char *argv[])
{	
	pthread_t tid1,tid2;

	sem_init(&sem1,0,1);
	sem_init(&sem2,0,0);

	pthread_create(&tid1,NULL,func1,NULL);
	pthread_create(&tid2,NULL,func2,NULL);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	return 0;
}
