//析构函数
#include <iostream>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;

class Complex{
private:
	double real;
	double imag;
public:
	Complex(double r = 0.0,double i = 0.0){
		cout<<"construction…"<<endl;
		real = r;	
		imag = i;
	}
	~Complex(){
		cout<<"destruction……(real,imag )= "<<real<<imag<<endl;
	}

	double absComplex(){
		double t;  
		t = real*real+imag*imag;
		return sqrt(t);
	}
};

int main()
{
	Complex A(1.1,2.2);
	cout<<"abs of complex A="<<A.absComplex()<<endl;
	
	
	{
		Complex B(3,4);//对象B的作用域就是这一对大括号
		cout<<"abs of complex B="<<B.absComplex()<<endl;
	}
	
	Complex *p = new Complex(5,6);
	cout<<"abs of complex *p="<<p->absComplex()<<endl;
	delete p;    //我们用delete手动释放对象指针时，同样会调用析构函数
	
	return 0;	
}
