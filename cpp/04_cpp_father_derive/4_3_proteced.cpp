//不涉及派生时，保护成员与私有成员的地位完全一致。
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Samp{
private:
	int a;
protected:	
	int b;
public:	
	int c;
	Samp(int n,int m){
		a = n;
		b = m;
	}
	int getA(){
		return a;
	}
	int getB(){
		return b;
	}
};

int main()
{	
	Samp obj(20,30);
	//obj.a = 11;		//Error,私有成员
	//obj.b = 22;		//Error,保护成员
	obj.c = 33;		//Ok
	cout<<obj.getA()<<' '<<obj.getB()<<endl;	//Ok
	return 0;
}
