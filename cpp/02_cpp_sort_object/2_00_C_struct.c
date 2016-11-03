// C结构体
#include <stdio.h>

struct Point
{
	int x;
	int y;
};

int main()
{
	struct Point a;
	a.x = 10;
	a.y = 20;
	
	printf("a.x = %d,a.y = %d\n",a.x,a.y);
	return 0;
}
