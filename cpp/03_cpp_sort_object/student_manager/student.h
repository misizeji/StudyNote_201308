#ifndef ____STUDENT___H____
#define ____STUDENT___H____

#include "../../03_cpp_sort_object/student_manager/string.h"

class Student{
protected:
	String name;
	static int total_num;
	static double total_score;
	int score;
	int num;

public:
	Student(char *str = "Error", int n = 0, double s = 0);
	Student(const Student&);
	void displayInfo();
	static void displayAverage();
};

#endif
