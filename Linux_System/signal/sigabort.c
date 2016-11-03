
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	sigset_t set;
	int sig_ret;
	int i = 0;
	sigemptyset(&set);
	sigaddset(&set,SIGABRT);
	sigprocmask(SIG_BLOCK,&set,NULL);
	for(i=0;i<5;i++)
	{
		sleep(1);
		printf("state:block\n");
		if (i == 2)
		{
			abort();
		}
	}
	
	sigprocmask(SIG_UNBLOCK,&set,NULL);
	for(i=0;i<5;i++)
	{
		sleep(1);

		printf("state:unblock\n");
	}	
	return 0;
}
