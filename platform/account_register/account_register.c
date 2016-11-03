#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

int main(int argc,char *argv[])
{

	char usr_name[20] = "xiaoguoguo";
	char pass_word[20] = "123456";
	
	//在数据库中查找账号是否存在
	sqlite3 *COUNT;	//数据库指针
	char *errmsg = NULL;
	char *sql = "select * from usr_list;";
	char **resultp = NULL;
	int nrow = 0;
	int ncolumn = 0;
	int i = 0;
	
	int ret = sqlite3_open("usr_list.db",&COUNT);
	if (ret !=SQLITE_OK)
	{
		perror("sqlite3_open");
	}
	
	sqlite3_get_table(COUNT,sql,&resultp,&nrow,&ncolumn,&errmsg);
	if (errmsg != NULL)
	{
		printf("%s\n",errmsg);
	}
	
	for (i = 2;i < (nrow*ncolumn+2); i++)//屏蔽了类别名称
	{
		if((i%2) == 0)//获得号码
		{	
			printf("__usr_name_%s\n",*(resultp + i));//i+1就能将密码取出来
			if(strcmp(usr_name,*(resultp + i)) == 0)
			{
				//数据库中有此账号
				printf("the user is %s\n",*(resultp + i));
				break;
			}
			else if(i == (nrow*ncolumn))//数据库中没有此人
			{
				printf("there is nobody,allow to register\n");
				char sqlite_insert[512] = "";
				sprintf(sqlite_insert,"insert into usr_list values ('%s','%s');",usr_name,pass_word);
				
				sqlite3_exec(COUNT,sqlite_insert,NULL,NULL,&errmsg);
				if (errmsg != NULL)
				{
					printf("%s\n",errmsg);
				}
				printf("register successful!!!\n");
				sqlite3_free_table(resultp);
				sqlite3_close(COUNT);
				return 0;
			}
		}
	}
	sqlite3_free_table(resultp);
	sqlite3_close(COUNT);
	printf("account exit!!!\n");
	return 0;
}
