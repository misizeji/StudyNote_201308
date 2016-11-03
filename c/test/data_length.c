/* ************************************************************************
 *       Filename:  data_length.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年12月18日 19时17分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#include <stdio.h>

int main(int argc, char *argv[])
{
	struct data{
		int a:3;
		unsigned int b;
	}DAT;


	DAT.a = 0x07;/*这里的0x07是以补码的形式存储的所以打印出 -1 */
	DAT.b = 3;
	printf("%d\n",DAT.a);
	printf("%d\n",DAT.b);
	return 0;
}

