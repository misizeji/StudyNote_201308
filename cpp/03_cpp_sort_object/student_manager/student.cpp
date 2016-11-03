#include <iostream>
using namespace std;

#include "student.h"

int Student::total_num = 0 ;
double Student::total_score = 0;

Student::Student(char *str, int n, double s):name(str),score(s)
{
	num = n;
	total_num ++;
	total_score += s;
}

Student::Student(const Student&ob):name(ob.name)
{
	num = ob.num;
	total_num ++;
	total_score += ob.score;
	score = ob.score;
}

void Student::displayAverage()
{
	double aver = (double)total_score/total_num;
	cout<<"total_num = "<<total_num<<endl;
	cout<<"平均成绩："<<aver<<endl;
}

void Student::displayInfo()
{
	cout<<num<<"\t"
	<<name.getStr()
	<<"\t"<<score<<endl; 
}
