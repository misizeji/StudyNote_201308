
#include <sys/types.h>
#include <signal.h>

#include <stdio.h>

void sig(int num)
{
	printf("you pressed ctrl+c\n");
}

int main(int argc, char *argv[])
{
	signal(SIGINT,sig);
	while(1);
	return 0;
}
