#include <iostream>

using std::cout;
using std::cin;
using std::endl;


class Point{
public:
	void setXY(int a, int b)
	{
		x = a;
		y = b;
	}
	int getX()
	{
		return x;
	}
	int getY()
	{
		return y;
	}
private:
	int x;
	int y;
};

class Line{
public:
	void setL(int a)
	{
		l = a;
	}
	int getL()
	{
		return l;
	}
private:
	int l;
};

class Face:public Point,public Line{  //以公有方式派生出Face
public:
	void setM(int a)
	{
		m = a;
	}
	int getM()
	{
		return m;
	}
private:
	int m;
};

int main()
{
	Point cc;
	cc.setXY(2,3);
	
	cout<<cc.getX()<<","<<cc.getY()<<endl;

	cout<<"------------------\n";
	Face f;//定义Face 对象
	f.setXY(4,5);
	f.setL(6);
	f.setM(7);
	cout<<f.getX()<<","<<f.getY()<<","<<f.getL()<<","<<f.getM()<<endl;
	return 0;
}