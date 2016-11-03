//对象作为类的数据成员
#include <iostream>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;

class MyString{
private:
	char *str;
public:
	MyString(char *s = "aa"){	
		str = new char[strlen(s)+1];
		strcpy(str,s);
		cout<<"构造MyString "<<str<<endl;
	}
	
	MyString(const MyString & t){
		str = new char[strlen(t.str)+1];
		strcpy(str,t.str);
		cout<<"拷贝构造MyString\n";
	}
	
	~MyString(){ 	
		cout<<"析构MyString\n";
		delete str; 
	}

	void print(){
		cout<<str<<endl;
	}
};    

class Girl{
private:
	MyString name;
	MyString address;
	int age;

public:
	Girl(char *st, char *ad, int ag):name(st),address(ad)
	{	
		age = ag;
		cout<<"构造girl\n";
	}

	Girl(const Girl & t):name(t.name)
	{
		age = t.age;
		cout<<"拷贝构造 girl"<<endl;
	}

	void print(){	
		name.print();
	   	cout<<"age:"<<age<<endl;
	}

	~Girl(){	
		cout<<"析构girl\n";
	}
};

int main()
{	
	MyString sss("hello");
	Girl  obj("chao hao","hhsdfsdf",8);
	//obj.print();
	Girl temp(obj);

	return 0;
}
