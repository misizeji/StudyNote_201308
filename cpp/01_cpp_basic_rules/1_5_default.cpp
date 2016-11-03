
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void func(int x,int y = 40,int z = 49);

int main()
{
   func(1,2,3);
   func(1,2);
   func(1);
   //func();
   return 0;
}

void func(int x,int y ,int z)
{
   cout<<x<<' '<<y<<' '<<z<<endl;
}


