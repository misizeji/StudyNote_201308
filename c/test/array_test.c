
#include <stdio.h>

int main(int argc, char *argv[])
{
	int a[] = {0,1,2,3,4,5,6,7,8,9};

	int num = ((a+3) - (a));

	printf("the address1 is %d\n",a);
	printf("the address1 num is %d\n",*a);
	printf("the address2 is %d\n",a+3);
	printf("the address2 num is %d\n",*(a+3));
	printf("the num is %d\n",num);
	return 0;
}
