#include <stdio.h>

#define MAX 20
#define _DEBUG
#define SetBit(x)  (x)
const int MIN=10;
static int N=10;
int roll(int n)
{
	int i=2.0;
	int f=1;
        if(n<=N)
        {
                while (i <= n)
                {
                        f = f * i;
                        i = i + 1;
                }
        }
	return f;
}
int main()
{
        printf("MAX+MIN = %d\n",MAX+MIN);
#ifdef _DEBUG
        printf("SetBit(6) = %d\n",SetBit(6));

#endif
        int  n,m;
        scanf("%d", &n);
	m=roll(n);
        printf("%d", m);
        return 0;
}

