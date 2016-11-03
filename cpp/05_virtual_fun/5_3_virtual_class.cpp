//抽象类
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Vehicle{  
protected:      
    float speed;
    int   total;     
public:     
    Vehicle(float speed,int total){    
        this->speed = speed;    
        this->total = total;
		cout<<"in Vehicle construct"<<endl;
    }    
	Vehicle(const Vehicle  & vv){
		cout<<"in Vehicle copy"<<endl;
	}

	virtual void ShowMember() = 0;//纯虚函数的定义

    virtual ~Vehicle(){}
};

class Car:public Vehicle{
protected:      
    int aird;   
	
public:
    Car(int aird,float speed,int total):Vehicle(speed,total){      
        this->aird = aird;    
		cout<<"in car construct"<<endl;
    }
    Car(Car & cc):Vehicle(cc){
		this->aird = cc.aird;
		cout<<"in Car copy "<<endl;
	}

    virtual  void ShowMember(){   //派生类成员函数重载  
        cout<<aird<<"|"<<speed<<"|"<<total<<endl;    
    }   
};      
  
int main()    
{  
	//Vehicle a(100,4);//错误,抽象类不能创建对象   
    Car b(250,150,4);
	cout<<"========"<<endl;
	Car c = b;
    b.ShowMember();  
	return 0;
}
