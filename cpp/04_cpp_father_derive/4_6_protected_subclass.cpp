//保护派生
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

class Derived:protected Base{		//保护派生类
private:
	int z;
public:
	void setXYZ(int a,int b,int m){ 
		setXY(a,b);
		z = m;
	}
	void show(){
		showXY();
		cout<<z<<endl;
	}
};


int main()
{
	Derived obj;
	//obj.y = 3;            //error
	//obj.setXY(10,20);		//error
	obj.setXYZ(10,20,30);			//ok
//	obj.showXY();			//error
	obj.show();				//okx
	return 0;
}
