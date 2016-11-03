#include <iostream>

using std::cout;
using std::cin;
using std::endl;

class Point{
public:
	Point(int a, int b)
	{
		x = a;
		y = b;
	}
	void display() const
	{
		//++x;
		//++y;
		cout<<"x = "<<x<<", y = "<<y<<endl;
	}
private:
	int x;
	mutable int y;
};

int main()
{
	Point aa(1,2);
	aa.display();
	return 0;
}