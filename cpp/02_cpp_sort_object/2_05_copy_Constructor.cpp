//拷贝构造函数及发生调用拷贝构造的情况
#include <iostream>
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;

class Point{
private:
	int x,y;
public:
	Point(int a,int b){   //构造函数
		x = a; 
		y = b;
		cout<<"构造 "<<x<<" "<<y<<endl;
	}

	Point(const Point &p){    	  //拷贝构造函数
		x = p.x; 
		y = p.y;
		cout<<"拷贝构造 "<<x<<" "<<y<<endl;
	}

	~Point(){
		cout<<"析构 "<<x<<" "<<y<<endl;
	}

	void print(){ 
		cout<<x<<' '<<y<<endl;
	}
};


void test (Point x)
{
	cout<<"in test fun ";
	x.print();
}


int main()
{	
	Point p1(30,40);//定义对象p1
	cout<<"after construate p1"<<endl;
	
	Point p2(p1);  //显式调用拷贝构造函数,通过对象p1创建对象p2
	p1.print();
	p2.print();
	//test(p1);
	//cout<<"after test"<<endl;
	return 0;	
} 
