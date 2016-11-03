// 使用静态成员函数访问静态数据成员
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class SmallCat{
private:
	double weight;
	static double total_weight;	//静态数据成员
	static double total_number;	//静态数据成员
public:
	SmallCat(double w){
		weight = w;
		total_weight += w;
		total_number++;
	}

	void display(){
		cout<<"小猫的重量是："<<weight<<"磅"<<endl;
	}

	static void totalDisp()	//静态成员函数
	{
		//cout<<weight<<endl;
		cout<<total_number<<"只小猫总重是："<<total_weight<<"磅"<<endl;
	}
};

/*void SmallCat::totalDisp()	//静态成员函数
{
//	cout<<weight<<endl;
	cout<<total_number<<"只小猫总重是："<<total_weight<<"磅"<<endl;
}*/

double SmallCat::total_weight = 0;
double SmallCat::total_number = 0;

int main()
{		
//	SmallCat::total_weight=0;
	SmallCat::totalDisp();	//通过类调用静态成员函数
	
	SmallCat w1(1.8),w2(1.6),w3(1.5);
	w1.display();
	w2.display();
	w3.display();
	SmallCat::totalDisp();	//通过类调用静态成员函数
	
	cout<<endl;
	w1.totalDisp();

	return 0;
}