// 定义对象数组时，通过初始化表调用
// 带有参数的构造函数。
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Exam{
private:
	int x;
	int y;
public:

	Exam(int a ,int b = 9 ){ 
		x = a; 
		y = b;
	}

	int getX(){ 
		return x; 
	}
	int getY(){
	 	return y; 
	}
};

int main()
{	

	Exam ob1[2] = {Exam(11),Exam(21,22)};	//初始化表

	for(int j = 0; j < 2; j++){
		cout<<ob1[j].getX()<<' '<<ob1[j].getY()<<endl;
	}

	return 0;
}
