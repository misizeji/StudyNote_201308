#ifndef __DOG_H__
#define __DOG_H__

#include <iostream>
using namespace std;

#include "../../05_virtual_fun/animal_sort/animal.h"

class dog:public Animal{
private:

public:
	dog(char *src):Animal(src)
	{
		cout<<"HELLO ! I am "<<str<<endl;
	}

	dog(dog &ob):Animal(ob)
	{
		cout<<"HELLO ! I am "<<ob.str<<endl;
	}

	virtual void sleep();
	virtual void eat();
	virtual void play();
};

#endif