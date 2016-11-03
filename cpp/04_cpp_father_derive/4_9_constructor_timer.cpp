//构造函数与析构函数的执行顺序
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Base{
private:
	int x;
public:
	Base(int i){ 
		x = i;
		cout<<"构造base类, x="<<x<<endl;
	}
	~Base(){ 
		cout<<"析构base类, x="<<x<<endl;
	}
	void show(){ 
		cout<<"x="<<x<<endl;
	}
};


class Derive:public Base{
private:
	int y;
	Base d;
public:
	Derive(int i,int j,int k):Base(i),d(j){ 
		y = k;
		cout<<"构造derived类, y="<<y<<endl;
	}
	~Derive(){ 
		cout<<"析构derived类, y="<<y<<endl;
	}
};

int main()
{
	Derive obj(1,2,3);
	obj.show();
	return 0;
}
