/* ************************************************************************
 *       Filename:  test.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月25日 11时01分59秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#include <stdio.h>


int main(int argc, char *argv[])
{
	char buf_1[1] = "";
	int buf_2[1] = {};
	printf("%s\n",buf_1);
	printf("%d\n",buf_1);

	printf("%d\n",buf_2[1]);
	printf("%d\n",buf_2);

	return 0;
}


