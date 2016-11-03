#include <iostream>
using namespace std;
class Circle
{
private:
	double radius;
public:
	Circle(double r) { 
		radius = r; 
	}
	void setR(double r) {
		radius = r; 
	}
	double getR() { 
		return radius;
	}
	double Area() {
		return 3.14*radius*radius; 
	}
	double Girth() {
		return 2*3.14*radius; 
		}
};

int main()
{
	Circle C(10);
	cout<<"圆的的面积为 "<<C.Area()<<endl;
	cout<<"圆的的周长为 "<<C.Girth()<<endl;
	return 0;
}