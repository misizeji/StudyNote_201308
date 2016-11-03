//使用指针作参数，完成两数互换
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void swap(int *m,int *n);
void swap(int &m,int &n);

int  main()
{	
	int a = 10;
	int b = 20;
	cout<<a<<' '<<b<<endl;
	
	swap(&a,&b);

	cout<<a<<' '<<b<<endl;
	cout<<"************************************\n\n";
	
	int aa = 10;
	int bb = 20;
	cout<<"aa value = "<<aa
		<<", bb value= "<<bb<<endl;
	cout<<"aa address= "<<&aa
		<<", bb address= "<<&bb<<endl<<endl;

	swap(aa,bb);
	
	cout<<"aa value = "<<aa
		<<", bb value= "<<bb<<endl;

	return 0;
}

void swap(int *m,int *n)
{	
	int temp;
	temp = *m;
	*m = *n;
	*n = temp;
}

void swap(int &m,int &n)
{
	cout<<"in fun 'swap(int &m,int &n)'"<<endl;
	cout<<"m addrress = "<<&m
		<<", n addresss = "<<&n<<endl;
	int temp;
	temp = m;
	m = n;
	n = temp;
	cout<<"swap(int &m,int &n) over"<<endl;
}