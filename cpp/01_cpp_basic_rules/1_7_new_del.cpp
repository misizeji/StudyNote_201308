//C++语言中的动态内存分配
#include <iostream>
#include <cstdio>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;

int main()
{
	int *p, *q;
	p = new int;
	q = new int(90);
	*p = 10;
	cout<< *p + *q <<endl;
	cout<<"p= "<<p<<endl;//打印指针的值，即指向的地址
	cout<<"&p= "<<&p<<endl;//打印指针变量本身在内存中的地址
	delete p,q;

	char *p_str = NULL;
	p_str = new char[100];
	memset(p_str,'a',100);
	//	p_str[99]='\0';
	cout<<p_str<<endl; //打印字符指针指向的内容
	cout<<&p_str<<endl;//打印指针变量本身在内存中的地址  
	cout<<(void *)p_str<<endl; //打印指针的值，即指向字符串的首地址地址
	delete p_str;

	return 0;
}