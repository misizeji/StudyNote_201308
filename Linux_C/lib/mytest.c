/* ************************************************************************
 *       Filename:  mytest.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2012年11月27日 11时58分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include <stdio.h>
#include "mylib.h"

int main(int argc, char *argv[])
{
	int a = 10, b = 20, max_num, min_num;
	max_num = max(a, b);
	min_num = min(a, b);
	printf("max = %d\n", max_num);
	printf("min = %d\n", min_num);
	return 0;
}


