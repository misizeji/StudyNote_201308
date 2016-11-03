/* ************************************************************************
 *       Filename:  malloc_length.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年12月19日 19时55分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	static int count = 1;
	char *p = NULL;
	
	while(1)
	{
		p = (char *)realloc(p,(count*1024*1024));
		if(p == NULL)
		{
			break;
		}
		count++;
	}

	printf("the malloc_length is %d \n",count-1);
	free(p);
	return 0;
}

