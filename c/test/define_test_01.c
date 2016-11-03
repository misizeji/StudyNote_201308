/* ************************************************************************
 *       Filename:  define_test_01.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年12月14日 16时15分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include <stdio.h>
#include <string.h>


#define	MYprintf_1(...)  printf(__VA_ARGS__)

#define	MYprintf_2(args...) printf(args)

#define	MYprintf_3(args) printf(args)

int main(int argc, char *argv[])
{
	MYprintf_1("hello world\n");
	MYprintf_2("my dear baby\n");
	MYprintf_3("come again baby\n");

	return 0;
}

