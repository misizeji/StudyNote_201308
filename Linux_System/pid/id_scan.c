/* ************************************************************************
 *       Filename:  id_scan.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月24日 10时13分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	pid_t pid, ppid, pgid;
	
	pid = getpid();
	ppid = getppid();
	pgid = getpgid(0);

	printf("pid is %d\n",pid);
	printf("ppid is %d\n",ppid);
	printf("pgid is %d\n",pgid);

	return 0;
}


