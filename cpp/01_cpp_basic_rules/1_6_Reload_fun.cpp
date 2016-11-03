//参数个数或参数类型不同的函数重载
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int mul(int x, int y, int z = 0)
{
	cout<<"in int mul(int x,int y,int z = 0 )\n";
	return x*y*z;
}

int mul(int x, int y)
{
	cout<<"in int mul(int x,int y)\n";
	return x*y;
}

double mul(int x,double y)
{
	cout<<"in double mul(int x,double y)\n";
	return x*y;
}


int main()
{
	int a = 3;
	int b = 4;
	double c = 5.66;
	cout<<a<<'*'<<c<<'='<<mul(a,c)<<endl;
	//cout<<a<<'*'<<b<<'='<<mul(a,b)<<endl;   //ERROR
	cout<<a<<'*'<<b<<'*'<<c<<'='<<mul(a,b,c)<<endl;
	
	return 0;
}






