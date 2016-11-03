//构造函数/析构函数执行顺序
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class XYZ{
private:
	int x;		//对象序号
public:
	XYZ(int x0){
		x = x0;
		cout<<"构造第"<<x<<"个对象"<<endl;
	}
	~XYZ(){
		cout<<"析构第"<<x<<"个对象"<<endl;
	}
};

void test();

int main()
{
	XYZ A(1),B(2);
	test();
	cout<<"                  "<<"after test"<<endl;
	return 0;
}

void test()
{
	cout<<"                  "<<"in test"<<endl;
	XYZ C(3);
	cout<<"                  "<<"after XYZ(3)"<<endl;
}
