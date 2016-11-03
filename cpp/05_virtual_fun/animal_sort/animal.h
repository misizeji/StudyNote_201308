#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include <iostream>

using namespace std;

class Animal{
protected:
	char *str;
public:
	Animal(char *src);
	Animal(Animal &P);
	~Animal();
	
	char *Get_str();
	char *Set_str(char *src);
	virtual void sleep() = 0;
	virtual void eat() = 0;
	virtual void play() = 0;
};

#endif
