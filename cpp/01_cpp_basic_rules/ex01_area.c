//��ϰ��������C���Գ����дΪC++����
#include <stdio.h>

float circle(float r)
{
	return (float)3.1415926*r*r;
}

int main()
{
	float r,s;
	printf("������Բ�İ뾶��");
	scanf("%f",&r);
	printf("\n");
	s = circle(r);
	printf("Բ������ǣ�%f\n",s);
	
	return 0;
}




