// 使用友元成员函数访问另一个类
#include <iostream>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;

class Girl;			//向前引用
//class Boy;
class Boy{
private:
	char *name;
	int age;
public:
	Boy(char *N,int A){
		name = new char[strlen(N)+1];
		strcpy(name,N);
		age = A;
	}
	void disp(Girl &);	//声明disp()为类boy的成员函数
	~Boy(){
		delete name;
	}
}; 

class Girl{
private:
	char *name;
	int age;
public:
	Girl(char *N,int A){
		name = new char[strlen(N)+1];
		strcpy(name,N);
		age = A;
	}
	friend void Boy::disp(Girl &);	//声明类boy的成员函数disp()为类girl的友元函数
	~Girl(){
		delete name;
	}
};

void Boy::disp(Girl &x)
{	
	cout<<"boy\'s name is:"<<name<<",age:"<<age<<endl;  //访问本类对象成员
	cout<<"girl\'s name is:"<<x.name<<",age:"<<x.age<<endl;  //访问友元类对象成员
}

int main()
{
	Boy b("chen hao",25);
	Girl g("zhang wei",18);
	b.disp(g);
	return 0;
}
