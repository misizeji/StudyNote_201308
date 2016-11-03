//虚析构函数 
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class ClxBase{
public:
    ClxBase(){
		cout<<"construct Base class!"<<endl;
	}

    virtual ~ClxBase(){
    	cout<<"in destructor of class ClxBase"<<endl;
    }

    void DoSomething(){
		cout << "!!!!Do something in class ClxBase!!!" << endl; 
	}
};


class ClxDerived : public ClxBase{
public:
    ClxDerived(){
		cout<<"construct Sub class!"<<endl;
	}
	
    ~ClxDerived(){
		cout << "in destructor of class ClxDerived!" << endl; 
	} 

    void DoSomething(){ 
		cout << "====Do something in class ClxDerived!====" << endl; 
	}
};


int main()
{
	ClxBase *pTest = new ClxDerived;
	pTest->DoSomething();
	delete pTest;
	cout<<"\nafter delete"<<endl;

	//ClxDerived aa;
	return 0;
}
