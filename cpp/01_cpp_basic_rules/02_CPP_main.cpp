//C++程序：MyMax.cpp

/*
#include <iostream> //C++规范

//using namespace std;

using std::cin;
using std::cout;
using std::endl;
*/

#include <iostream.h>		//C 规范

float max(float a,float b);

int main()
{ 	
	float x,y,z = 1.2f;

	cout<<"输入两个数:";//输出到显示器
	cin>>x>>y;	// 从键盘输入
	z = max(x,y);
	cout<<"最大数是: ";
	cout<<z<<endl;

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

