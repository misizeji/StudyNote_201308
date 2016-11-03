
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

struct Point{
	int x, y;
public:
	void setXY(int w,int t){
		x = w;
		y = t;
	}

	void print(){
		cout<<x<<' '<<y<<endl;
	}
};

int main()
{
	Point a;
	a.x = 11;
	a.y = 17;
	//a.setXY(12,18);
	a.print();

	return 0;
}
