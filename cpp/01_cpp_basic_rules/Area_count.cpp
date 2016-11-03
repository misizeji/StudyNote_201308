#include <iostream>

using namespace std;

float func(int x,float y = 30,float PI = 3.14);

int main (void)
{
	int r;
	cout<<"please input R = ";
	cin>>r;
	float Area = func(r);

	cout<<"Area = "<<Area<<endl;
	return 0;
}

float func(int x,float y,float PI)
{	
	return PI * (x * x) * (y/360);
}