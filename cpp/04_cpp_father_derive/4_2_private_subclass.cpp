//私有继承
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class base{		//基类
private:
	int x;
	int y;
public:
	void setXY(int n,int m){
		x = n;
		y = m;
	}
	void showXY(){
		cout<<x<<','<<y<<endl;
	}
};

class derived:private base{		//私有派生类
private:
	int z;
public:
	void set(int n,int m,int r){  
		setXY(n,m);
		z = r;
	}
	void show(){ 
		cout<<z<<endl;
		showXY();
		//cout<<x<<','<<y<<','<<z<<endl;		//?
	}
};

int main()
{
	derived obj;
//	obj.setXY(10,20);						//?
	obj.set(10,20,30);
	obj.show();
	return 0;
}
