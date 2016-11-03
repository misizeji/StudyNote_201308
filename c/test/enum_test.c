
#include <stdio.h>
#include <string.h>

//枚举值是常量，不是变量
enum week{sun,mon,tue,wed,thu,fri,sat};

int main(int argc, char *argv[])
{
	int i = 0;
	//定义枚举类型的变量
	enum week monday,weekday;
	//对枚举变量赋值
	monday = mon;	
	for (i=0;i<7;i++)
	{
		if(i == sun)
		{
			printf("today is sunday!!\n");
			printf("sun's num is %d\n",sun);
		}
	}
	return 0;
}
