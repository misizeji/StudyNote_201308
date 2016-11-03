#include <iostream>
#include <cstring>
using namespace std;

#include "animal.h"

Animal::Animal(char *src)
{
	str = new char[strlen(src)+1];
	strcpy(str,src);
}

Animal::Animal(Animal &P)
{
	str = new char[strlen(P.str)+1];
	strcpy(str,P.str);
}

Animal::~Animal()
{
	delete str;
}

char *Animal::Get_str()
{
	return str;
}
char *Animal::Set_str(char *src)
{
	str = new char[strlen(src)+1];
	strcpy(str,src);
	return str;
}



