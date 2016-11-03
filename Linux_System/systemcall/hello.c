/* ************************************************************************
 *       Filename:  hello.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月22日 11时05分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
	int fd = 0;
	fd = open("./txt", O_CREAT | O_WRONLY);
    if (-1 == fd)
		perror ("open");
	int ret = 0;
	ret = write(fd, "helloworld\n", 11);
	if (-1 == ret)
		perror("write");
	return 0;
}


