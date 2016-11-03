#include <iostream>      

using std::cin;
using std::cout;
using std::endl;     
      
class Internet{  
public:  
    Internet(char *name,char *url){  
        this->name = new char[strlen(name)+1];  
        this->url = new char[strlen(url)+1];  
        if(name){  
            strcpy(this->name,name);  
        }  
        if(url){  
            strcpy(this->url,url);  
        }  
    }  
    Internet(Internet &temp){  
        Internet::name = new char[strlen(temp.name)+1];  
        Internet::url  = new char[strlen(temp.url)+1];  
        if(name){  
            strcpy(this->name,temp.name);  
        }  
        if(url){  
            strcpy(this->url,temp.url);  
        }  
		cout<<"copy\n";
    }  
    ~Internet(){  
        delete[] name;  
        delete[] url;  
    }  
    Internet & operator = (const Internet &temp)          //赋值运算符重载函数  
    {  
		cout<<"this->name="<<this->name<<" temp.name="<<temp.name<<endl;
        delete[] this->name;  
        delete[] this->url;  
		this->name = new char[strlen(temp.name)+1];  
        this->url = new char[strlen(temp.url)+1];  
        if(this->name){  
            strcpy(this->name,temp.name);  
        }  
        if(this->url){  
            strcpy(this->url,temp.url);   
        }  
        return *this;  
    }  
public:  
    char *name;  
    char *url;  
};  

int main()  
{    
    Internet a("凌阳教育","www.sunplusedu.com");  
    Internet b = a;				//b对象还不存在，所以调用拷贝构造函数，进行构造处理。  
    cout<<"b "<<b.name<<" "<<b.url<<endl;  
    
	Internet c("大学计划","www.unsp.com");
    b = c;						//b对象已经存在，所以系统选择赋值运算符重载函数处理。  
    cout<<"b "<<b.name<<" "<<b.url<<endl<<endl;  
	return 0;
}
