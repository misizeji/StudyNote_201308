
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char dest[12] = "";
	char *src_1 = "hello world";
	//char src_2[12] = "heihe hahei";
	char *src_2 = "heihe hahei";

	strcpy(dest,src_1);
	printf("___1___%s\n",dest);
	bzero(dest,sizeof(dest));
	strcpy(dest,src_2);
	printf("___2___%s\n",dest);
	return 0;
}
