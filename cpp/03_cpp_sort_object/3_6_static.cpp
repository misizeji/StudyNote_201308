//静态数据成员
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Student{
public:	
	static int count;	//声名为静态数据成员
	int StudentNo;
public:
	Student(){
		count++;
		StudentNo = count;
	}
	void print(){
		cout<<"Student "<<StudentNo<<" ";
		cout<<"count = "<<count<<endl;
	}
};


int Student::count;	//定义静态数据成员并初始化

int main()
{	
	cout<<"use count = "<<Student::count<<endl;
	
	Student s1;
	s1.print();
	cout<<"------\n";

	Student s2;
	s1.print();
	s2.print();
	cout<<"------\n";

	Student s3;
	s1.print();
	s2.print();
	s3.print();
	cout<<"------\n";
	return 0;
}
