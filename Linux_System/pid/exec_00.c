/* ************************************************************************
 *       Filename:  exec_00.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月24日 14时51分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[])
{

	execl("/bin/ls","ls","-l","-a","-h",NULL);

	return 0;
}


