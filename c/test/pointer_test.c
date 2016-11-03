/* ************************************************************************
 *       Filename:  pointer_test.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年12月18日 09时06分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include <stdio.h>


int main(int argc, char *argv[])
{
	int a = 0;
	char buf[] = "hello world";
	
	a =(int)buf;
	printf("in the pointer: %s\n",(char *)a);
	printf("in the address: %s\n",buf);
	return 0;
}



