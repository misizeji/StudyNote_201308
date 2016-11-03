//作用域
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int avar = 10;						    //全局变量avar

int main()
{
	int avar = 20;						//局部变量avar	//句1
	cout<<"avar is: "<<avar<<endl;	    //访问局部变量
	avar = 25;							//修改局部变量	//句2
	cout<<"avar is: "<<avar<<endl;	

	cout<<"avar is: "<<::avar<<endl;	    //访问全局变量
	::avar = 30;						    //修改全局变量
	cout<<"avar is: "<<::avar<<endl;	

	return 0;
}
