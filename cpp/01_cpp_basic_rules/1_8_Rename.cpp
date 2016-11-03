//“引用”的工作原理
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main()
{	
	int i;
	int &j = i;	   //j是i的引用
	i = 30;
	cout<<"i = "<<i<<" j = "<<j<<endl;
	j = 80;
	cout<<"i = "<<i<<" j = "<<j<<endl;
	cout<<"Address of i = "<<&i<<endl;
	cout<<"Address of j = "<<&j<<endl;

	return 0;
}
