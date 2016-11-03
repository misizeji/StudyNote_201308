#include <iostream>
using namespace std;

#include "person.h"

void person::sleep()
{
	cout<<"person ("<<str<<") is sleeping...!!"<<endl;
}

void person::eat()
{
	cout<<"person ("<<str<<") is eating...!!"<<endl;
}
	
void person::play()
{
	cout<<"person ("<<str<<") is playing...!!"<<endl;
}

void person::play_with(Animal &P)
{
	cout<<str<<" play with "<<P.Get_str()<<endl;
	P.play();
}