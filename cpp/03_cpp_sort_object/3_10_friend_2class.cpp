//使用一个友元函数访问两个不同的类
#include <iostream>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;

class Boy;			//向前引用

class Girl{
private:
	char name[25];
	int age;
public:
	void init(char N[],int A);
	friend void prdata(const Girl plg,const Boy plb);	//声明函数为girl类的友元函数
};									

void Girl::init(char N[],int A)
{	
	strcpy(name,N);
	age = A;
}

class Boy{
private:
	char name[25];
	int age;
public:
	void init(char N[],int A);
	friend void prdata(const Girl plg,const Boy plb);	//声明函数为boy类的友元函数
};

void Boy::init(char N[],int A)
{	
	strcpy(name,N);
	age = A;
}

void prdata(const Girl plg,const Boy plb)
{	
	cout<<"女孩"<<plg.name<<','<<plg.age<<"岁；";
	cout<<"男孩"<<plb.name<<','<<plb.age<<"岁。\n";
}

int main()
{	
	Girl G1;
	Boy B1;
	G1.init("Stacy",12);
	B1.init("Jim",11);

	prdata(G1,B1);
	return 0;
}


