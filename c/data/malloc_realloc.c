
#include <stdio.h>
#include <string.h>


#define	MAXSIZE 3

typedef struct {
	int * elem; //指向内存中的第一个元素
	int length;//已使用数据元素的个数
	int listsize;//数据元素的总个数
}Sqlist;
/*初始化一个动态链表申请MAXSIZE 个int类型数据的内存空间*/
void initSqlist(Sqlist * L)
{
	L->elem = (int *)malloc(MAXSIZE * sizeof(int));
	if(L->elem == NULL)	//malloc申请空间失败
		exit(-1);
	L->length = 0;//使用个数为零
	L->listsize = MAXSIZE;//申请总个数为MAXSZIE个单位数据结构
}
insertElem(Sqlist *L,int i, int item)
{
	//在表L中的第i个位置，插入item
	int *base,*insertptr,*p;
	if(i < 0|| i > L->length)
	{
		printf("插入的位置不对\n");
		exit(0);
	}
	if(L->length == L->listsize)
	{
		//追加申请新的内存空间
		base = (int *)realloc(L->elem,(L->listsize+1)*sizeof(int));
		L->elem=base;
		L->listsize++;
	}
	insertptr = &(L->elem[i]);//得到插入地点的地址
	if(i < L->length)//插入点为前面的某个位置的时候
	{
		for(p = &(L->elem[L->length - 1]);p >= insertptr;p--)
		{
			//从最后一个元素往后挪
			*(p+1) = *p;
		}
	}
	*insertptr = item; //插入要插入的数
	L->length++;//插入的数目加1
}

int main(int argc, char *argv[])
{

	return 0;
}
