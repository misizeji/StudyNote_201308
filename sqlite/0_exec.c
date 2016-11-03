
#include <stdio.h>
#include "sqlite3.h"

int main(int argc, char *argv[])
{
	sqlite3 *db;
	int ret = 0;
	char *errmsg = NULL;
	char *sql = "insert into haha values (3,'weige','male');";

	ret = sqlite3_open("./test.db",&db);
	if (ret !=SQLITE_OK)
	{
		perror("sqlite3_open");
	}

	sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if (errmsg != NULL)
	{
		printf("%s\n",errmsg);
	}

	sqlite3_close(db);
	return 0;
}
