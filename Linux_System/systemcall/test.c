/* ************************************************************************
 *       Filename:  test.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月22日 20时15分23秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (JT), 
 *        Company:  
 * ************************************************************************/

#include <stdio.h>

int main(int argc, char *argv[])
{
	char buf[128] = "";
	sscanf("123abcABC","%[1-9A-Z]",buf);
	printf("%s\n",buf);
	return 0;
}

