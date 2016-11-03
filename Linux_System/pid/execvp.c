


#include <stdio.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
	

	char *arg[] = { "ls", "-l", "-a", "-h", NULL };

	execvp("ls",arg);

	perror("execvp");

	return 0;
}
