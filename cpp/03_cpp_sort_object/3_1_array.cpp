//对象数组： 
//			类名	数组名［数组大小］;
//引用方式： 
//			数组名［下标］.成员名

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Exam{
private:
	int x;
public:
	void setX(int n){ 
		x = n; 
	}
	int getX(){ 
		return x;
	}
};

int main()
{	
	Exam ob[4];				//对象数组的定义
	int i;
	for(i = 0; i < 4; i++)	
		ob[i].setX(i);		//引用
	
	for(i = 0; i < 4; i++)	
		cout<<ob[i].getX()<<endl;
	
	return 0;
} 
