//例2.6 内联函数
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

#define doub(x)  x*2

inline int inlineDoub(int x) 
{
	return x*2;
}

int main()
{ 
	int i;
	//使用宏定义
	for(i = 1;i <= 3; i++){
		cout<<i<<" doubled is "<<doub(i)<<endl;		
	}

	cout<<"1+2 doubled is "<<doub(1+2)<<endl;

	cout<<endl;
	
	//使用内联函数
	for(i = 1; i <= 3; i++){
		cout<<i<<" doubled is "<<inlineDoub(i)<<endl;
	}
	cout<<"1+2 doubled is "<<inlineDoub(1+2)<<endl;
	
	return 0;
}
