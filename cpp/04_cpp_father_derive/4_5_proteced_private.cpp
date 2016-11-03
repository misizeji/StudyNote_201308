//构造函数与析构函数的执行顺序
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Base{
protected:
	int a;
public:
	void setA(int sa){
		a = sa;
	}
};


class Derive1:private Base{
protected:
	int b;
public:
	void setB(int sb){
		b = sb;
	}
};

class Derive2:public Derive1{
private:
	int c;
public:
	void setC(int sc){
		c = sc;
	}
	void show(){
		cout<<a<<' '<<b<<' '<<c<<endl;
		//cout<<b<<' '<<c<<endl;
	}
};

int main()
{
	Base op1;
	op1.setA(1);
	
	Derive1 op2;
	op2.setB(2);
	
	Derive2 op3;
	op3.setC(3);
	op3.setB(4);

	op3.show();
	return 0;
}
