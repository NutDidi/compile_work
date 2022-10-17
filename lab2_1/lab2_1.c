#include <stdio.h>
#include <stdlib.h>
#define X 14
#define Y 7

const int N=4;

int parameter(int n)
{
    int i;
    i = 40;
    while (i>=0)
    {
        if (n % 3 >= 2 || n / 5 < 8)
        {
            n = n + 1;
            continue;
        }
        else if (n * 2 <= 70)
        {
            n = n + 2;
            break;
        }
        else
        {
            n = n + 3;
            i = i - 1;
        }
    }
    return n;
}

int main()
{
    int n,result1;
    scanf("%d", &n);
    result1 = parameter(n);
    int a[4];
    a[0]=X+Y;
    a[1]=X-Y;
    a[2]=X+Y*X-Y;
    a[3]=rand()%10+a[0]+a[1];
    if (result1 != a[3] ) printf("result1:%d\n",result1);
    if (a[2] == a[0] * a[1]) printf("X+Y*X-Y=(X+Y)*(X-Y)");
    else printf("X+Y*X-Y<>(X+Y)*(X-Y)");
    return 0;
}

