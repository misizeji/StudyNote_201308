/* ************************************************************************
 *       Filename:  malloc_test.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年12月19日 08时09分26秒
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
	char *p = NULL;
	int i = 0;
	int i_before = 0;
	p = (char *)malloc(0);

	if(strcmp("",p) == 0)
		printf("heihei\n");
	if(p == NULL)
		printf("haha\n");

	
	printf("sizeof(*p) is %d\n",sizeof(*p));	/* sizeof 在堆中是不可使用的，*p代表一个字符 */
	printf("sizeof(p) is %d\n",sizeof(p));		/* p是一个指针，任何指针的大小都是4个字节 */
	printf("strlen is %d(strlen)\n",strlen(p));
	printf("the ascii is %d\n",*p);
	printf("the p string is %s\n",p);                 /* 返回的是一个空指针 */
	
	for(i=0;i<20;i++)
	{
		strncpy(p,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",i);
		if((strlen(p) != (i_before+1)) && (i>0))
		{
			printf("the i_before(also normal length) is %d (strlen)\n",i_before);
			printf("the abnormal length is %d (strlen)\n",strlen(p));
			printf("the abnormal str is %s\n",p);
			break;
		}

		i_before = i;

		printf("the length is %d (strlen)\n",strlen(p));
		printf("the str is %s\n",p);
	}
	return 0;
}


