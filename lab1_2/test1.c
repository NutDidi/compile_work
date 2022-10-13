#include <stdio.h>
static int N=10;
#define MAX 20
#define MIN 10
#define _DEBUG
#define SetBit(x)  (1<<x)
int main()
{
	printf("hello,world\n");
	printf("MAX = %d , MIN = %d , MAX+MIN = %d\n",MAX,MIN,MAX+MIN);
#ifdef _DEBUG
	printf("SetBit(5) = %d , SetBit(6) = %d\n",SetBit(5),SetBit(6));
#endif
	int i, n, f;
	scanf("%d", &n); 
	i = 2;
	f = 1;
	if(n<=N)
	{
		while (i <= n)
		{
			f = f * i;
			i = i + 1;
		}
	}
	printf("%d", f);
	return 0;
}
