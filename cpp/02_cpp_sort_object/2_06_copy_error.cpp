#include <iostream>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;

class StringData{
private:  
	char *str;
public:
	StringData(char *s);
	
	StringData(const StringData &p){
		str = new char[strlen(p.str)+1];
		strcpy(str, p.str);
		cout<<"拷贝构造函数"<<' '<<str<<endl;
	}

	~StringData(){
		cout<<"析构函数"<<' '<<str<<endl;
		delete str;
	}

	char * getStr(){
		return str;
	}
}; 

int main()
{
	StringData x("AAAAAAA"); 
	StringData y = x;

    //cout<<"return test = "<<test(x).getStr()<<endl;
	cout<<"main over"<<endl;
	return 0;
}

StringData::StringData(char *s)
{   
	cout<<"构造函数"<<' '<<s<<endl;
	str = new char[strlen(s)+1];
	if(str != 0) 
		strcpy(str,s);
}

StringData test(StringData p)
{
	cout<<"in test "<<p.getStr()<<endl;
	return p;
}
