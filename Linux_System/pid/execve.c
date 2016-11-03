


#include <stdio.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
	

	char *arg[] = { "test", NULL };
	char *env[] = {"USER=LINUX",NULL}; 
	execve("./test",arg,env);

	perror("execvp");

	return 0;
}
