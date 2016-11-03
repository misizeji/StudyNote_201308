#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct link
{
	int num;
	struct link *next;
	
}LINK;

//insert before head without order
LINK *link_insert(LINK *head,LINK *pi)
{
	pi->next = head;
	head = pi;
	return head;
}

void link_print(LINK *head)
{
	LINK *pi = head;
	while(pi != NULL)
	{
		printf("%d ",pi->num);
		pi = pi->next;
	}
	printf("\n");
	return ;
}
//change the head to the end and change the end to head
LINK *link_reverse(LINK *head)
{
	LINK *pb, *pt;
	
	if(head == NULL)		//link链表不存在
	{
		return head;
	}
	else
	{
		pb = head->next;	//pb指向第二个节点
		head->next = NULL;	//现在的头是以后的尾部尾部next为NULL
		
		while(pb != NULL)
		{
			pt = pb->next;	//开始时pb指向的是第三个节点
			pb->next = head;
			head = pb;
			pb = pt;
		}
	}
	return head;
}

//change the link to order with num primary key
LINK *link_order(LINK *head)
{
	LINK *pb = NULL,*pdata_max = NULL;
	
	if(head == NULL)
	{
		return head;
	}
	
	pdata_max = head;

	while(pdata_max->next != NULL)	//from big to small; 
	{
		pb = pdata_max->next;
		while(pb != NULL)
		{
			//just change the values !!
			if(pb->num > pdata_max->num)
			{
				int temp = pdata_max->num;
				pdata_max->num = pb->num;
				pb->num = temp;
			}
			pb = pb->next;
		}
		pdata_max = pdata_max->next;//change the bigest to the next;
	}
	return head;
}

//the main function
int main(int argc,char *argv[])
{
	LINK *head = NULL;
	
	while(1)
	{
		char buf[20] = "";
		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf) - 1] = '\0';
		if((strncmp(buf,"quit",4)) == 0 )
		{
			break;
		}
		LINK *pi = NULL;
		pi = (LINK *)malloc(sizeof(LINK));
		pi->next = NULL;
		
		pi->num = atoi(buf);
		head = link_insert(head,pi);
		
	}
	
	link_print(head);	//顺序打印
	//head = link_reverse(head);	//逆序
	//link_print(head);	//逆序打印
	head = link_order(head);
	link_print(head);		
	
	return 0;
}