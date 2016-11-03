#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Circle{

private:
	float r;
	float area;
	float cir_length;
	float PI;
public:
	Circle(float a = 0,float PI_in = 3.14)
	{
		r = a;
		PI = PI_in;
	}
	
	float GetArea()
	{	
		return PI * (r * r);
	}
	
	float Get_Cirlength()
	{
		return PI * r * 2;
	}
	
	void Disp_Info()
	{
		cout<<"The Area = "<<GetArea()<<endl;
		cout<<"The Cirlength = "<<Get_Cirlength()<<endl;
	}
	~Circle()
	{
		cout<<"in the ~Circle"<<endl;
		delete r;
		delete area;
		delete cir_length;
		delete PI;
	}
};

int main()
{
	Circle P(3);
	P.Disp_Info();
	return 0;
}