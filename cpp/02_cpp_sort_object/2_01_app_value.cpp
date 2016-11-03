// 对象之间的赋值
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class MyClass{
private:
	int a,b;
	char *p;
public:
	void set(int i,int j){
		a = i;
		b = j;
	}
	void show(){
		cout<<a<<','<<b<<endl;
	}
};

int main()
{	
	MyClass p1;
	MyClass p2;
	p1.set(20,5);
	//p2 = p1;			//对象赋值
	p1.show();
	//p2.show();  
	return 0;
}