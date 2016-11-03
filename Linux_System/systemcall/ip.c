/* ************************************************************************
 *       Filename:  ip.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月22日 17时11分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#include<stdio.h>

int main()
{

	char *host = "192.168.220.5";
	char ipaddr [16];
	int p1, p2, p3, p4;
	sscanf ( host ,"%d.%d.%d.%d", &p1, &p2, &p3, &p4);
	sprintf (ipaddr,"%s\n",host);
	printf ("%d  %d  %d  %d\n",p1,p2,p3,p4);
	printf ("%s",ipaddr);

	return 0;
}
