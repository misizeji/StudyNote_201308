#ifndef __CAT_H__
#define __CAT_H__

#include <iostream>
using namespace std;

#include "../../05_virtual_fun/animal_sort/animal.h"

class cat:public Animal{
private:

public:
	cat(char *src):Animal(src)
	{
		cout<<"HELLO ! I am "<<str<<endl;
	}
	
	cat(cat &ob):Animal(ob)
	{
		cout<<"HELLO ! I am "<<ob.str<<endl;
	}
	
	virtual void sleep();
	virtual void eat();
	virtual void play();
};

#endif