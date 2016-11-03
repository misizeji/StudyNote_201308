#include <iostream>
#include <cstring>
using namespace std;

#include "student_fur.h"

Student_Fur::Student_Fur(char *str,int a,double b,char *tec):Student(str,a,b)
{
	Student_Fur::teacher = new char[strlen(tec)+1];   
    if(teacher)  //分配成功
        strcpy(Student_Fur::teacher,tec);
}


Student_Fur::Student_Fur(const Student_Fur &ob):Student(ob)
{
	Student_Fur::teacher = new char[strlen(ob.teacher)+1];   
    if(teacher)  //分配成功
        strcpy(Student_Fur::teacher,ob.teacher);
}

Student_Fur::~Student_Fur()
{
	delete teacher;
}
void Student_Fur::print_msg()
{
	cout<<num<<"\t"<<Student::name.getStr()<<"\t"<<score<<"\t"<<teacher<<endl;
}



