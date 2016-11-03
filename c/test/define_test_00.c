/* ************************************************************************
 *       Filename:  define_test_00.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年12月14日 12时04分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include <stdio.h>
#include <string.h>


//#undef  DEBUG
#define DEBUG /* 注释后就代表 #undef DEBUG */

#ifdef  DEBUG
#define	debugPrintf(...) printf("DEBUG:"__VA_ARGS__)
#else      /* -----  not DEBUG  ----- */
//#define debugPrintf(...) printf("%s", "")
#define debugPrintf(...) NULL
#endif     /* -----  not DEBUG  ----- */

/* 以上调整debug信息的方法看似很不错，但又是很愚蠢的做法
 * 因为我们生成代码的时候也还是会把 #undef DEBUG情况下的
 * 那些 printf("%s","") 代码编译链接到可执行程序中
 * 其实要想做得更好，就应该这样：
 * #ifdef DEBUG
 * printf("DEBUG:"__VA_ARGS__) 
 * __VA_ARGS__代表想要打印的调试信息
 * #endif                                               */

#define STR(x) #x	

#define	SET_DATA(x,n) SET_##x(n)
/*
	#error 强迫编译程序停止编译，主要用于程序调试 
*/

//#define	


int SET_x(int n)
{
	return n;
}

int SET_y(int n)
{
	return n;
}

int main(int argc, char *argv[])
{
	debugPrintf("hello world\n");
	
	printf(STR(Programming in C is fun:\n));
	
	int ret = SET_DATA(x,10);
	printf("%d\n",ret);
	
	ret = SET_DATA(y,11);
	printf("%d\n",ret);
	
	printf("%d\n",__LINE__);
	printf("%s\n",__FILE__);
	printf("%s\n",__DATE__);
	printf("%s\n",__TIME__);
	printf("%d\n",__STDC__);
	
	return 0;
}

