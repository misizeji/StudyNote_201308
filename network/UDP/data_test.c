#include <stdio.h>
int main(int argc, char *argv[])
{
	union {
		short s;
		char c[sizeof(short)];
		}un;
		
		un.s = 0x0102;
		if( (un.c[0] == 1) && (un.c[1] ==2) )
		{
			printf("big \n");
		}
		else if( (un.c [0] == 2) && (un.c [1] == 1) ) 
		{	
			printf("small\n");
		}
		return 0;
}
 
 

 