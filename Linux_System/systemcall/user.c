/* ************************************************************************
 *       Filename:  user.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月22日 14时36分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <utmp.h>

int main()
{
	struct utmp usr;
	int fd = -1;
	fd = open ("/var/run/utmp",O_RDONLY);
	if (-1 == fd)
		perror ("open");
	while (1)
	{
        int ret = read (fd, &usr, sizeof(usr));
		if (ret == 0)
		{
			break;
		}
		else if (usr.ut_type == USER_PROCESS && (ret != 0))
    	{

	    	printf("%s\t%s\t\t%s\n",usr.ut_user,usr.ut_line,ctime(&usr.ut_tv.tv_sec));
    	}
	}
	return 0;
}


