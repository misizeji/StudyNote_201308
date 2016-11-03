
#include <stdio.h>

static int a = 0;

void func()
{
	a++;
	printf("in the func = %d\n",a);
	return;
}


int main(int argc, char *argv[])
{
	func();
	func();
	a++;
	printf("in the main = %d\n",a);
	return 0;	
}


