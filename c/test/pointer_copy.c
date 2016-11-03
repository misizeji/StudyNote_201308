#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char num[20] = "18612491570";
	char msg[20] = "123456";
	char *data = "18612491570+123456";

	sscanf(data,"%[^+]+%s",num,msg);
	
	printf("%s\n",num);
	printf("%s\n",msg);
	return 0;
}
