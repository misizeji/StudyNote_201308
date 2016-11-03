
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	sigset_t set;
	int sig_ret;

	sigemptyset(&set);

	sig_ret = sigismember(&set,SIGINT);

	printf("sig_ret is %d\n",sig_ret);

	sigaddset(&set,SIGINT);

	sig_ret = sigismember(&set,SIGINT);

	printf("sig_ret is %d\n",sig_ret);

	sigfillset(&set);
	return 0;
}
