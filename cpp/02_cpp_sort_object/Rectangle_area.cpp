#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class MyClass{
private:
	int x;	//position (x, y);
	int y;
	int width;	
	int height;
	
public:
	MyClass(int x_a = 0,int y_b = 0,int width_c = 0,int height_d = 0)
	{
		x = x_a;
		y = y_b;
		width = width_c;
		height = height_d;
	}
	void SetWidth(int width_c)
	{
		width = width_c;
	}
	void SetHeight(int height_d)
	{
		height = height_d;
	}
	void GetX(int x_a)
	{
		x = x_a;
	}
	void GetY(int y_b)
	{
		y = y_b;
	}
	void GetWidth(int width_c)
	{
		width = width_c;
	}
	void GetHeight(int height_d)
	{
		height = height_d;
	}
	int Area(int width_c,int height_d)
	{
		return width_c * height_d;
	}
	void DispInfo()
	{
		cout<<"Area = "<<Area(width,height)<<endl;
		cout<<"width = "<<width<<" height = "<<height<<endl;
		cout<<"the position is ("<<x<<','<<y<<')'<<endl;
	}
};

int main()
{
	MyClass P;
	
	P.DispInfo();
	P.GetX(1);
	P.GetY(2);
	P.SetWidth(6);
	P.SetHeight(8);
	P.DispInfo();
	
	MyClass Q(0,0,10,20);
	Q.DispInfo();

	return 0;
}