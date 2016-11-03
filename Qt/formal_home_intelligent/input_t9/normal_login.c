#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

/********************************************************
*�������ƣ��û�������¼����
*�����������û���������
*��������ֵ������ -1���ļ��򿪴��� 0����¼ʧ�� 1����¼�ɹ�
*********************************************************/
int normal_login(char *usr_name,char *pass_word)
{

	//char usr_name[20] = "root";
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
			if((strcmp(usr_name,*(resultp + i)) == 0) &&((strcmp(pass_word,*(resultp + i+1))) == 0))
			{
				//���ݿ����д��˺���������ͬ
				//printf("the user is %s\n",*(resultp + i));
				//printf("the password is  %s\n",*(resultp + i+1));
				break;
			}
			else if(i == (nrow*ncolumn))//���ݿ���û�д���
			{
				//printf("there is nobody\n");
				sqlite3_free_table(resultp);
				sqlite3_close(COUNT);
				return 0;
			}
		}
	}
	sqlite3_free_table(resultp);
	sqlite3_close(COUNT);
	return 1;
}
