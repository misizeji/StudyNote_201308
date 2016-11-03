#include <iostream>
using std::cout;
using std::endl;

enum bits{one=1,two=2,four=4};
enum bigstep{first,second=100,third};
enum {zero,null=0,size,num=1};//匿名枚举类型

class A{
public:
	//匿名枚举类型
	enum {SIZE1=100,SIZE2=200};
	int array[SIZE1];
};

class B{
public:
	enum bits{one=1,two=2,four=4};
	bits flag;//等价 int flag;
};

class MyColor{
public:
	enum color{white=1,black,red,green,blue};
	void setColor(color color)
	{
		currentColor = color;
	}
	color getColor(){ return currentColor; }
private:
	color currentColor;
};
int main()
{
	MyColor color;
	color.setColor(MyColor::black);
	cout<<color.getColor()<<endl;
	return 0;
}