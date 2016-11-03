#ifndef __PERSON_H__
#define __PERSON_H__

#include <iostream>
using namespace std;

#include "../../05_virtual_fun/animal_sort/animal.h"

class person:public Animal{
private:
	int age;
public:
	person(char *src,int age):Animal(src)
	{
		cout<<"HELLO I am "<<src<<endl;
		this->age = age;
	}
	person(person &ob):Animal(ob)
	{
		this->age = ob.age;
	}
	
	virtual void sleep();
	virtual void eat();
	virtual void play();
	void play_with(Animal &P);
};

#endif