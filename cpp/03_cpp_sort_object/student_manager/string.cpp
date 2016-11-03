#include <iostream>
#include <cstring>      
using namespace std;     

#include "string.h" 
      
String::String(char *str)  
{  
    String::str = new char[strlen(str)+1];   
    if(str)  //分配成功
        strcpy(String::str,str);  
}  

String::String(const String &temp)  
{   
    String::str=new char[strlen(temp.str)+1];  
    if(str)  
        strcpy(String::str,temp.str);  
}  

String::~String()  
{  
    if(str)
		delete[] str;  
}  

void String::append(char *s)
{
	char *p;
	p = new char[strlen(s)+strlen(str)+1];
	strcpy(p,s);
	strcat(p,str);
	delete str;
	str = p;
}  

int String::stringLong()
{
	return strlen(str);
}

void String::set(char *s)
{
	delete str;
	str = new char[strlen(s)+1];
	strcpy(str,s);
}

void String::print()
{
	cout<<str;
}

char* String::getStr()
{
	return str;
}

String& String::operator =(const String&ob)	//重载函数进行赋值
{
	delete str;
	str = new char[strlen(ob.str)+1];
	strcpy(str,ob.str);
	return *this;
}