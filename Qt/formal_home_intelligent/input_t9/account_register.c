#include <stdio.h>
#include <string.h>
#include "sqlite3.h"


/******************************************************
*�������ƣ�QT�û��˺�ע�ắ��
*�����������û���������
*��������ֵ������ 1���ɹ� 0��ʧ��
********************************************************/
int account_register(char *usr_name,char *pass_word)
{

	//char usr_name[20] = "xiaoguoguo";
	//char pass_word[20] = "123456";
	
	//�����ݿ��в����˺��Ƿ����
	sqlite3 *COUNT;	//���ݿ�ָ��
	char *errmsg = NULL;
	char *sql = "select * from usr_list;";
	char **resultp = NULL;
	int nrow = 0;
	int ncolumn = 0;
	int i = 0;
	
	int ret = sqlite3_open("../../www/cgi-bin/usr_list.db",&COUNT);
	if (ret !=SQLITE_OK)
	{
		perror("sqlite3_open");
		return -1;
	}
	
	sqlite3_get_table(COUNT,sql,&resultp,&nrow,&ncolumn,&errmsg);
	if (errmsg != NULL)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	
	for (i = 2;i < (nrow*ncolumn+2); i++)//�������������
	{
		if((i%2) == 0)//��ú���
		{	
			//printf("__usr_name_%s\n",*(resultp + i));//i+1���ܽ�����ȡ����
			if(strcmp(usr_name,*(resultp + i)) == 0)
			{
				//���ݿ����д��˺�
				//printf("the user is %s\n",*(resultp + i));
				break;
			}
			else if(i == (nrow*ncolumn))//���ݿ���û�д���
			{
				//printf("there is nobody,allow to register\n");
				char sqlite_insert[512] = "";
				sprintf(sqlite_insert,"insert into usr_list values ('%s','%s');",usr_name,pass_word);
				
				sqlite3_exec(COUNT,sqlite_insert,NULL,NULL,&errmsg);
				if (errmsg != NULL)
				{
					printf("%s\n",errmsg);
				}
				//printf("register successful!!!\n");
				sqlite3_free_table(resultp);
				sqlite3_close(COUNT);
				return 1;
			}
		}
	}
	sqlite3_free_table(resultp);
	sqlite3_close(COUNT);
	printf("account exit!!!\n");
	return 0;
}
