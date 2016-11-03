#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "gprs.h"

int main(int argc,char *argv[])
{

	char num[20] = "18076488386";
	
	//在数据库中查找账号是否存在
	sqlite3 *COUNT;	//数据库指针
	char *errmsg = NULL;
	char *sql = "select * from pass_list;";
	char **resultp = NULL;
	int nrow = 0;
	int ncolumn = 0;
	int i = 0;
	
	int ret = sqlite3_open("login_count.db",&COUNT);
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
			printf("__id_%s\n",*(resultp + i));
			if(strcmp(num,*(resultp + i)) == 0)//数据库中有此账号
			{
				break;
			}
			else if(i == nrow*ncolumn)//数据库中没有此人
			{
				printf("there is nobody\n");
				sqlite3_free_table(resultp);
				return 0;
			}
		}
	}
	
	printf("____the i = %d\n",i);	//i+1就能将密码取出来
	sqlite3_free_table(resultp);
	sqlite3_close(COUNT);
	//gprs_login_num_send(num);//调用gprs模组进行发送短信
	return 0;
}
