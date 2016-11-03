// 使用友元函数 
#include <iostream>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;

class Girl{
private:
	char *name;
	int age;
public:
	Girl(char *n,int d){	
		name = new char[strlen(n)+1];
		strcpy(name,n);
		age = d;
	}

	friend void disp(Girl &x);	//声明为友元函数
//	{	
//		cout<<"girl\'s name is:"<<x.name<<",age:"<<x.age<<endl;
//	}

	~Girl(){
		delete name;
	}
};

void disp(Girl &x)				//定义友元函数
{	
	cout<<"girl\'s name is:"<<x.name<<",age:"<<x.age<<endl;
}

int main()
{	
	Girl e("Chen Xingwei",18);
	disp(e);					//调用友元函数
	return 0;
}
