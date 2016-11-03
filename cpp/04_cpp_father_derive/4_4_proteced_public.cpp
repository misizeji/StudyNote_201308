//基类中的保护成员在公有派生的派生类中仍是保护的
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Base{
protected:
	int a, b;
public:
	void setAB(int n,int m){
		a = n;
		b = m;
	}
};

class Derive:public Base{
private:
	int c;
public: 
	void setC(int n){
		c = n;
	}
	void showABC(){
		cout<<a<<' '<<b<<' '<<c<<endl; 
	}
};

int main()
{	
	Derive obj;
	//obj.a = 3;
	obj.setAB(2,4);
	obj.setC(3);
	obj.showABC();
	return 0;
}
