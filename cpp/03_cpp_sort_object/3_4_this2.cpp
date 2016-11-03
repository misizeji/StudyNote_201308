//this 指针使用2
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class ABC{
public:
	int x;
public:
	//ABC(ABC *this, int x = 9)
	ABC(int x = 9){
		this->x = x;
		cout<<"in ABC"<<endl;
	}
	//ABC(ABC *this,const ABC &tt)
	ABC(const ABC &tt){
		this->x = tt.x;
		cout<<"in Cype ABC"<<endl;
	}
	//~ABC(ABC *this)
	~ABC(){
		cout<<"in ~ABC"<<endl;
	}
	//ABC& set(ABC *this,int x)
	ABC& set(int x){  //形参与数据成员同名     
		this->x = x;  
		return *this;
	}	
	//void show(ABC *this)
	void show(){   
		cout<<"in show x = "<<x<<endl;
	}
};

int main()
{
	ABC p;
//	p.set(5);
//	ABC t = p.set(5);

//	cout<<&(p.set(5))<<endl;
	p.set(5).show();
//	p.set(6);
//	p.show();

	cout<<"over main"<<endl;
	return 0;
}
