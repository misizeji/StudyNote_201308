
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

typedef void(*sighandler_t)(int);

void fun1(int num)
{
	printf("in fun1\n");
}
void fun2(int num)
{
	printf("in fun2\n");
}
void fun3(int num)
{
	printf("in fun3\n");
}

int main(int argc, char *argv[])
{
	sighandler_t ret = NULL;
	signal(SIGINT,fun1);
	ret = signal(SIGQUIT,fun3);
	if(ret == fun1)
	{
		printf("fun3 match\n");
	}
	ret = signal(SIGINT,fun2);
	if(ret == fun1)
	{
		printf("fun2 match\n");
	}
	while(1);
	return 0;
}
