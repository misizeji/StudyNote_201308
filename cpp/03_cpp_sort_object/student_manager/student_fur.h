#ifndef   __STUDNET_FUR_H__
#define   __STUDNET_FUR_H__

#include "../../03_cpp_sort_object/student_manager/student.h"

class Student_Fur:public Student{

private:
	char *teacher;
public:
	Student_Fur(char *str = "student",int a = 0,double b = 0,char *tec = "teacher");
	Student_Fur(const Student_Fur& ob);
	~Student_Fur();
	void print_msg();
};

#endif