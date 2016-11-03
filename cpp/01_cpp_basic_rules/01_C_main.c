/*C程序：MyMax.c*/
#include <stdio.h>

float max(float a,float b);

int main()
{  
	float x,y,z;
	printf("输入两个数:");
	scanf("%f%f",&x,&y);
	z = max(x,y);
	printf("最大数是：%f\n",z);
	
	return 0;
}

float max(float a, float b)
{ 
	float c;
	if(a > b) 
		c = a;
	else 
		c = b;
		
	return c;
}
