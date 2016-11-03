
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	system ("ls -lah");
	
	perror("system");

	return 0;
}
