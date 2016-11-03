#include <stdio.h>
#include "gprs.h"

int main(int argc,char *argv[])
{

	char num[20] = "13683604485";
	char msg[256] = "aaaaaa";

	gprs_deal(num,msg);//调用gprs模组进行发送短信

	return 0;
}
