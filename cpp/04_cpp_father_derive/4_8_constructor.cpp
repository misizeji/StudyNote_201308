//派生类的构造函数与析构函数的执行顺序
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Base{
public:
	Base(){
		cout<<"Construction base class.\n";//1
	}
	~Base(){
		cout<<"Destruction base class.\n";//4
	}
};

class Derive:public Base{
public:
	Derive(){
		cout<<"Construction derived class.\n";//2
	}
	~Derive(){
		cout<<"Destruction derived class.\n";//3
	}
};

int main()
{
	Derive op;
	return 0;
}
