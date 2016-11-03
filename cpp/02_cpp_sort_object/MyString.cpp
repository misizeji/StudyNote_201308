#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class MyString{
private:
	char *str;		//字符串指针
	int length;		//字符串长度
	
public:
	MyString(char *src)	//构造函数
	{
		int i = 0, j = 0;
		while(*(src+i) != '\0')
		{
			i++;
		}
		str = new char[i+1];
		for(j = 0;j < (i+1);j++)
		{
			str[j] = src[j];
		}
		cout<<"in the build"<<endl;
	}
	MyString(const MyString &P)	//拷贝构造函数
	{
		length = P.length;
		str = new char[length];
		int i = 0;
		for(i=0;i<length;i++)
		{
			str[i] = P.str[i];
		}
		cout<<"in the copy build"<<endl;
	}
	
	char *SetString(char *src)	//设置字符串
	{
		cout<<"in the SetString"<<endl;
		int i = 0, j = 0;
		while(*(src+i) != '\0')
		{
			i++;
		}
		str = new char[i+1];
		for(j = 0;j < (i+1);j++)
		{
			str[j] = src[j];
		}
		return str;
	}
	
	char *GetString()	//获得字符串
	{
		return str;
	}
	
	void Print_String()	//打印字符串
	{
		cout<<"the string is "<<str<<endl;
	}
	
	int Get_Length()	//获得字符串长度
	{
		length = 0;
		while(*(str+length) != '\0')
		{
			length++;
		}
		length = length +1;
		return length;
	}
	
	char Get_Char(int i)	//获得字符串中哪个字符
	{
		if(i <= Get_Length())
		{
			return *(str+i);
		}
		else
		{
			cout<<"the select is "<<i<<", select wrong!!"<<endl;
			return '\0';
		}
	}
	
	~MyString()	//析构函数
	{
		delete str;
	}

};


int main()
{
	cout<<"__________________"<<endl;	
	MyString A("hello");
	A.Print_String();
	
	cout<<"__________________"<<endl;	
	MyString B("world");
	B.Print_String();
	cout<<"get the string is "<<B.GetString()<<endl;
	cout<<"the length is "<<B.Get_Length()<<endl;
	cout<<"the second char is "<<B.Get_Char(1)<<endl;
	
	cout<<"__________________"<<endl;	
	MyString C(B);
	C.Print_String();
	C.SetString("heihei");
	cout<<"get the string is "<<C.GetString()<<endl;
	cout<<"the length is "<<C.Get_Length()<<endl;
	cout<<"the second char is "<<C.Get_Char(8)<<endl;
	
	return 0;
}
