#include <stdio.h>



int sum(int m)
{
	 int i = 0, n = 0;

	for ( i = 1; i <= m; i += 1 )
		n += i;

	printf("The sum of 1-m is %d\n", n);

	return 0;
}


int main(int argc, char *argv[])
{
	int i = 0, n = 0;
	sum(50);

	for ( i = 1; i <= 50; i += 1 )
		n += i;

	printf("The sum of 1-50 is %d\n", n);
	return 0;
}

