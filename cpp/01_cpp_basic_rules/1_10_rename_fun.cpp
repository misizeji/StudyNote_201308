//函数返回值为引用
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int a[] = {1,3,5,7,9};
int &index(int);	//声明返回引用的函数

int main()
{
	index(2) = 25;  //a[2] =25;
	cout<<a[2]<<endl;
	return 0;
}

int& index(int i)
{
	return a[i];
}
