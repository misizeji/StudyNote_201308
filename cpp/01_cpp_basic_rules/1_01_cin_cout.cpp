//C++的输入输出
#include <iostream>
#include <iomanip>
#include <stdio.h>

/*using std::cin;
using std::cout;
using std::endl;*/

using namespace std;

int main()
{
	int i;
	float f;
	// 换行
	cout<<"hello,this is a test!"<<"\n";
	cout<<"please input two num:";
	// 连续输入输出
	//cin>>i;
	//cin>>f;	
	cin>>i>>f;
	cout<<"i = "<<i
	   <<",f = "
	   <<setiosflags(ios::fixed)    //固定的浮点显示
	   <<setprecision(2)<<f<<'\n';  //有两个小数位
	// 格式控制
	cout<<"Dec i = "<<dec<<i<<endl;   //10进制显示
	cout<<"Hex i = "<<hex<<i<<endl;   //16进制显示
	cout<<"Oct i = "<<oct<<i<<endl;   //8进制显示
	cout<<endl;
	//字符串的输入输出
	char c = getchar();
	//cout<<"c hex = "<<hex<<(int)c<<endl;
	char name[20];
	cout<<"Hello,your name:";
	//cin>>name;
	cin.getline(name,sizeof(name)); //获取一行内容
	cout<<"*"<<name<<"*"<<endl;

	return 0;
}
