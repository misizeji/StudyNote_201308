#include <stdio.h>


#define MYSIZE(p)  sizeof(p)/sizeof(*p)

int main(int argc, char *argv[])
{
	char buf[] = "hello world !!!";

	printf("_____%d\n",MYSIZE(buf));
	printf("%c\n",*buf);
	printf("%s\n",buf);
	return 0;
}
