#ifndef __STRING__H__
#define __STRING__H__

class String{
  
private:  
	char *str;   
public:  
	String(char *str);
	String(const String &temp);
	~String();	//因为有指针所以需要析构释放内存
	char* getStr();
	void append(char *s);	//追加函数
	int stringLong();
	void set(char *s);
	void print();
	String& operator =(const String& );
};  

#endif