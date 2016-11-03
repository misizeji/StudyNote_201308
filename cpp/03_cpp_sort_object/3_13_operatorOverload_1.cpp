
//按照特定形式，改变函数入口
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class MyString{  
public:  
    MyString(char *str=""){  
       this->str = new char[strlen(str)+1];
	   if(this->str != NULL)
		   strcpy(this->str,str);
    }  
	 ~MyString(){
		 delete str;
	 }
	 void display(){cout<<str<<endl;}
	friend void append(MyString& ,MyString& );
	friend void operator + (MyString& ,MyString& );
private:  
    char *str;  
};  

void append(MyString& temp1,MyString& temp2)
{
	int len1 = strlen(temp1.str);
	int len2 = strlen(temp2.str);
	char *tmp = new char[len1+len2+1];
	if(tmp != NULL)
	{
		strcpy(tmp,temp1.str);
		strcat(tmp,temp2.str);
		delete temp1.str;
		temp1.str = tmp;
		cout<<"app over"<<endl;
	}
} 

void  operator + (MyString& temp1,MyString& temp2) 
{  
	int len1 = strlen(temp1.str);
	int len2 = strlen(temp2.str);
	char *tmp = new char[len1+len2+1];
	if(tmp != NULL)
	{
		strcpy(tmp,temp1.str);
		strcat(tmp,temp2.str);
		delete temp1.str;
		temp1.str = NULL;
		temp1.str = tmp;
	}
} 

int main()  
{  
    MyString a("abc");  
	MyString b("123");
	append(a,b);
	cout<<"after append(a,b) a.str =";
	a.display();
	a+b;
	cout<<"after a+b a.str =";
	a.display();
	return 0;
} 
