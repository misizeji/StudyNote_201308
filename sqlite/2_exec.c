
#include <stdio.h>
#include "sqlite3.h"

int main(int argc, char *argv[])
{
	sqlite3 *db;
	int ret = 0;
	char *errmsg = NULL;
	char *sql = "select * from haha;";
	char **resultp = NULL;
	int nrow = 0;
	int ncolumn = 0;
	int i = 0, j = 0;

	ret = sqlite3_open("./test.db",&db);
	if (ret !=SQLITE_OK)
	{
		perror("sqlite3_open");
	}

	sqlite3_get_table(db,sql,&resultp,&nrow,&ncolumn,&errmsg);
	if (errmsg != NULL)
	{
		printf("%s\n",errmsg);
	}
	
	int temp = ncolumn;
	
	for (i = 0;i < nrow; i++)
	{
		//printf("the row is %d\n",i);
		for(j = 0;j < ncolumn;j++)
		{
			/*********其实这个二维数组已经转成一个一位数组*********/
			/*  _________________________________________ */
			/* |0   1     2   3   4    5   6    7    8   |*/
			/* |id  name  sex 2 peter male 3 heihei male |*/
			/*  ``````````````````````````````````````````*/
			/***********************************************************
			二级指针的一位数组里面的存都是二级地址：所以能存多类型的数据
			数字与字符都可以存进去；取出时不能使用两个**
			************************************************************/
			
			//printf("the temp is %d\n",temp);
			printf("%s: %s\n",*(resultp + j),*(resultp + temp));
			temp++;
		}
	}

	sqlite3_free_table(resultp);
	sqlite3_close(db);
	return 0;
}
