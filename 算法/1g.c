#include <stdio.h>

int k,c;

int ef(int num)
{
	int sum=num;
	int a = k;
	while(num/a != 0 && sum < c)
	{
		sum += num/a;
		a *= k;
	}
	if(sum >= c)
		return num;
	else
		return 0;
}

int main()
{
	int v;
	int mid;
	scanf("%d %d",&c,&k);
	int n = c;
	int i = 0;
	while(i <= n)
	{
		mid = (i+n)/2;
		v = ef(mid);
		if(v>0)
			n = mid - 1;
		else
			i = mid + 1;
	}
	if(v==0)
		printf("%d\n",mid+1);
	else
		printf("%d\n",mid);
}
