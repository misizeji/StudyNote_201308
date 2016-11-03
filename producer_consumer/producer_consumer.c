
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

/*	 买1个生产2个	 */
int now_num = 3;	//初始时仓库中剩余3个产品
sem_t sem1,sem2;	//信号量实现同步
pthread_mutex_t mutex;	//互斥锁实现互斥

int garage(int flag)
{
	int i = 0;
	pthread_mutex_lock(&mutex);//上锁
	for(i = 0;i < 2;i++)
	{
		sleep(1);
		if(flag == 1)	//代表生产者
		{
			now_num = now_num + 1;
			if(now_num > 10)
			{
				now_num = 10;
				break;
			}
			printf("add into; the total num is %d\n",now_num);
		}
	}
	if(flag == 0)	//代表消费者
	{
		if (now_num != 0) //仓库里为0，不能消费
		{
			now_num--;
			printf("carry out;the total num is %d\n",now_num);
			sleep(1);
		}
		else
		{
			sleep(1);
			return 0;
		}
	}
	pthread_mutex_unlock(&mutex);//解锁
	return 0;
}
void *func1(void *arg)	//生产者
{
	while(1)
	{
		sem_wait(&sem1);
		garage(1);
		sem_post(&sem2);
	}
}

void *func2(void *arg) //消费者
{
	while(1)
	{
		sem_wait(&sem2);
		garage(0);
		sem_post(&sem1);
	}
}
int main(int argc, char *argv[])
{	
	pthread_t tid1,tid2;

	pthread_create(&tid1,NULL,func1,NULL);	//创建2个线程
	pthread_create(&tid2,NULL,func2,NULL);

	pthread_mutex_init(&mutex,NULL);	//初始化互斥锁	

	sem_init(&sem1,0,0);	//初始化2个信号量
	sem_init(&sem2,0,1);
		
	pthread_join(tid1,NULL);	//回收2个线程的资源
	pthread_join(tid2,NULL);

	pthread_mutex_destroy(&mutex);		//销毁互斥锁
	return 0;
}
