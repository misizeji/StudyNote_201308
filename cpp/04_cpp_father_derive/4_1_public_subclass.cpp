//公有派生
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Base{		//基类
private:
	int x;
public:
	int y;
	void setXY(int n,int m){
		x = n;
		y = m;
	}
	void showXY(){
		cout<<x<<','<<y<<endl;
	}
};

class Derived:public Base{		//公有派生类
private:
	int z;
public:
	void setZ(int m){ 
		z = m;
	}
	/*void show(){
		cout<<x<<','<<y<<','<<z<<endl;
	}*/
};


int main()
{
	Derived obj;
	obj.y = 3;
	obj.setXY(10,20);		//ok
	obj.setZ(30);			//ok
	obj.showXY();			//ok
	//obj.show();				//okx
	return 0;
}
