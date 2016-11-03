#ifndef  __SINGLE_TRACK_LINK__
#define  __SINGLE_TRACK_LINK__

struct student{
	int num;
	char name[20];
	int score;
	struct student *next;
};

typedef struct student STU;

extern struct STU *link_insert(struct STU *head,struct STU *temp); 	/*链表插入函数*/
extern struct STU *link_delete(struct STU *head,int num);		/*链表删除函数*/
extern int link_search(struct STU *head,int num);		/*链表查找函数*/
extern void link_print(struct STU *head);				/*链表打印函数*/
extern int link_free(struct STU *head);				/*链表释放函数*/
extern struct STU *link_order(struct STU *head);				/*链表排序函数*/


#endif