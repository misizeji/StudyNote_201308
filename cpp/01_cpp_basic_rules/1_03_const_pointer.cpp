//const与指针
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main()
{
	char name[20] =    "student"; 
	char company[20] = "sunplusedu";

	//指向常量的指针变量
	const char* p1 = name;
	name[1] = 'o';   
	//p1[1] = 'o';   //ERROR
	p1 = company;
	cout<<"name = "<<name<<endl;
	cout<<"p1 = "<<p1<<endl;
	cout<<endl;

	//常(量)指针
	char* const p2 = name;
	p2[1] = 't';
	//p2 = company;   //ERROR
	cout<<"p2 = "<<p2<<endl;
	cout<<endl;

	//指向常量的常(量)指针
	const char * const p3 = name;
	//	p3[1] = 'o';
	//	p3 = company;

	//指针指向常量
	char* p4 = "embedded engineer";
	//p4[0] = 'E';    //ERROR 
	cout<<"p4 = "<<p4<<endl;

	return 0;
}