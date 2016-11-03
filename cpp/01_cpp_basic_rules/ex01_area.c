//练习：将下列C语言程序改写为C++程序
#include <stdio.h>

float circle(float r)
{
	return (float)3.1415926*r*r;
}

int main()
{
	float r,s;
	printf("请输入圆的半径：");
	scanf("%f",&r);
	printf("\n");
	s = circle(r);
	printf("圆的面积是：%f\n",s);
	
	return 0;
}




