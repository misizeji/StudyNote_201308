#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "single_track_link.h"


int main()
{	
	struct STU *p = NULL;
	struct STU *head = NULL;
	
	struct STU xiaoming;
	xiaoming->num = 1;
	xiaoming->name[20] = "zhanzhao";
	xiaoming->score = 55;
	
	p = malloc(sizeof(xiaoming));
	p = &xiaoming;
	
	head = link_insert(head,p); 	/*链表插入函数*/
	
	return 0;
}