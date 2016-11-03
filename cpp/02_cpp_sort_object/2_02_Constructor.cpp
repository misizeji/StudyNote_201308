//构造函数
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
	Complex(double a, double b){	//构造函数
		real = a;
		imag = b;
		cout<<"In Constructor t!"<<endl;
	}
	
	double absComplex(){	
		double t;
	  	t = real*real+imag*imag;
	  	return sqrt(t);
	}
};

void fun(Complex p)	//参数为一个对象
{
	cout<<"end of fun()"<<endl;
}

int main()
{
	Complex A(1.1, 2.2);	  //定义类的对象A时调用构造函数complex
	//Complex B;
	cout<<"after constructor A"<<endl;
	cout<<"abs of complex A="<<A.absComplex()<<endl;
	
	//Complex *q = new Complex;
	Complex *p = new Complex(3.0,4.0);  //定义类的指针时在为之分配动态内存时会调用构造函数Complex
	cout<<"abs of complex *p="<<p->absComplex()<<endl;

	fun(Complex(1.0,1.0));	//传递一个无名的临时对象
	delete p;
	return 0;
}