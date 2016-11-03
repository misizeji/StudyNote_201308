
#include <stdio.h>
#include "sqlite3.h"

//typedef int (*exechandler_t)(void *para,int n_column,char **column_value,char **column_name);
//exechandler_t func1;
int func1(void *para,int n_column,char **column_value,char **column_name)
{
	int i = 0;
	for(i = 0;i < n_column;i++)
	{
		printf("%s\t:%s\n",column_name[i],column_value[i]);
	}
	return 0;
}
int main(int argc, char *argv[])
{
	sqlite3 *db;
	int ret = 0;
	char *errmsg = NULL;
	char *sql = "select * from haha;";

	ret = sqlite3_open("./test.db",&db);
	if (ret !=SQLITE_OK)
	{
		perror("sqlite3_open");
	}

	sqlite3_exec(db,sql,func1,NULL,&errmsg);
	if (errmsg != NULL)
	{
		printf("%s\n",errmsg);
	}

	sqlite3_close(db);
	return 0;
}
