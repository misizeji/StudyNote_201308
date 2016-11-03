#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "single_track_link.h"


struct STU *link_insert(struct STU *head,struct STU *temp)
{
	struct STU *pi = temp;
	if(head == NULL)
	{
		printf("the link is empty!!!\n");
		return NULL;
	}
	else
	{
		pi->next = head;
		head = pi;
	}
	return head;
}

struct STU *link_delete(struct STU *head,int num)
{
	return head;
}

int link_search(struct STU *head,int num)
{
	return 0;
}

void link_print(struct STU *head)
{
	return;
}

int link_free(struct STU *head)
{
	return 0;
}

struct STU *link_order(struct STU *head)
{
	return head;
}