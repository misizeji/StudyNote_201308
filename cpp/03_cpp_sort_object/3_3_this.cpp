//this指针
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Exam{
private:
	int x;
public:
	void load(int va1){
		this->x = va1;//与 x  =val等价
		cout<<"load--- this value: "<<this<<endl;
	}

	int getX()const
	{
		cout<<"getx--- this value: "<<this<<endl;
		return this->x;//与return x;等价
	}

	/*
	int getX(const Exam *this)
	{
		this->x = val;
	}*/

};

int main()
{
	Exam ob1,ob2;
	cout<<"ob1 address: "<<&ob1<<endl;
	ob1.load(100);
	cout<<ob1.getX()<<endl;

	cout<<"ob2 address: "<<&ob2<<endl;
	ob2.load(200);
	cout<<ob2.getX()<<endl;
	return 0;
}
