#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Point{
private:
	double x, y;
public:
	Point(double i, double j){
		x = i;
		y = j;
	}

	double Area() const{
		return 0.0;
	}
};

class Rectangle:public Point{
private:
	double w, h;
public:
	Rectangle(double i,double j,double k,double l);
	double Area() const{
		return w * h;
	}
};

Rectangle::Rectangle(double i,double j,double k,double l):Point(i, j){
	w = k;
	h = l;
}

void fun(Point &s){
	cout<<s.Area()<<endl;
}

int main(){

	Rectangle rec(3.0,5.2,15.0,25.0);
	fun(rec);

	return 0;
}
