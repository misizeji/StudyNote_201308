#include <iostream>
#include "student_fur.h"

using std::cout;
using std::cin;
using std::endl;

int main()
{
	// Student stu[3]={Student("xiaowang", 0, 90),
					// Student("xiaozhang", 1, 80.5),
					// Student("xiaolili",2, 87)};

	// cout<<"学号\t"<<"姓名\t\t"<<"成绩"<<endl<<endl;
	// Student ob(stu[0]);
	// for(int i = 0; i < 3; i++)
		// stu[i].displayInfo();
	// ob.displayInfo();
	// cout<<endl;
	// Student::displayAverage();

	// cout<<endl;
	
	Student_Fur stu[3]={Student_Fur("xiaowang", 0, 90,"haha"),\
					Student_Fur("xiaozhang", 1, 80.5,"heihei"),\
					Student_Fur("xiaolili",2, 87,"hehe")};	//对象数组

	cout<<"学号\t"<<"姓名\t\t"<<"成绩\t"<<"导师"<<endl;
	Student_Fur ob(stu[0]);	//验证拷贝构造
	for(int i = 0; i < 3; i++)
	{
		stu[i].print_msg();
	}
	ob.print_msg();	//打印拷贝构造结果
	//Student_Fur::name.displayAverage();	//name是保护的调用的时候相当于private是不允许外部调用的
	return 0;
}