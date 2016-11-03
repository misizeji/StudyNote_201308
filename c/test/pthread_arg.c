#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
/**************************************************************************************
编写一个多线程的程序：
	要求:
	创建2个子线程，主进程要传递3个参数给线程1，传递另一个参数给线程2；
	线程1、线程2中对每个传给线程的参数加1，等线程都执行完毕后,进程打印出四个参数的值。
	传给线程1参数的值必需要能够改变，传给线程2参数的值必需不能改变
***************************************************************************************/
void *fun1(void *argv[])
{
	
	*((int *)argv[0]) = *((int *)argv[0]) + 1;
	*((int *)argv[1]) = *((int *)argv[1]) + 1;
	*((int *)argv[2]) = *((int *)argv[2]) + 1;
	
	printf("in the fun 1 a = %d\n", *((int *)argv[0]));
	printf("in the fun 1 b = %d\n", *((int *)argv[1]));
	printf("in the fun 1 c = %d\n", *((int *)argv[2]));

	return NULL;
}

void *fun2(void *arg)
{
	int d = (int)arg;
	d = d + 1;
	printf("in the fun 2 d = %d\n", d);

	return NULL;
}

int main(void)
{

	int a = 1,b = 2,c = 3,d = 4;
	int *argv[] = {&a,&b,&c};
	int arg = d;
	
	printf("before a = %d\n", a);
	printf("before b = %d\n", b);
	printf("before c = %d\n", c);
	printf("before d = %d\n", d);
	pthread_t tid1,tid2;
	
	pthread_create(&tid1,NULL,(void *)fun1,(void *)argv);
	pthread_create(&tid2,NULL,(void *)fun2,(void *)arg);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	printf("after a = %d\n", a);
	printf("after b = %d\n", b);
	printf("after c = %d\n", c);
	printf("after d = %d\n", d);
	
	return 0;
}