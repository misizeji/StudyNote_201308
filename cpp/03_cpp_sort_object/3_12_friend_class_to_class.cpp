//boy为girl的友元类
#include <iostream>
#include <cstring>

using std::cin;
using std::cout;
using std::endl; 

class Girl;

class Boy{
private:
	char *name;
	int age;
public:
	Boy(char *n,int d){	
		name = new char[strlen(n)+1];
		strcpy(name,n);
		age = d;
	}
	void disp(Girl &);		//声明disp()为类boy的成员函数
	void test(Girl & g);

	~Boy(){
		delete name;
	}
};

class Girl{
private:
	char *name;
	int age;
	friend class Boy;			//声明类boy是类girl的友元
public:
	Girl(char *n,int d){
		name = new char[strlen(n)+1];
		strcpy(name,n);
		age = d;
	}
	~Girl(){
		delete name;
	}
};

void Boy::disp(Girl &x)		//定义函数disp()为类boy的成员函数，也是类girl的友元函数
{	
	cout<<"boy\'s name is:"<<name<<",age:"<<age<<endl;
	cout<<"girl\'s name is:"<<x.name<<",age:"<<x.age<<endl;
}

void Boy::test(Girl & g)
{
	cout<<"in boy test "<<g.name<<endl;
}

int main()
{	
	Boy b("chen hao",25);
	Girl g("zhang wei",18);
	b.disp(g);
	b.test(g);
	return 0;
}
