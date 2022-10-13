#include<stdio.h>
#define A 0
#define B 1
int main()
{
	int a,b,i,t,n;
	a=0;
	b=1;
	i=1;
	//input round
	scanf("%d",&n);
	//output initial value
	printf("a=%d\n",a);
	printf("b=%d\n",b);
	while(i<n)
	{
		t=b;
		b=a+b;
		printf("%d\n",b);
		a=t;
		i=i+1;
	}
	return 0;
}

