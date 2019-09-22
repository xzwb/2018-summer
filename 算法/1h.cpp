#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;

int power(int a,int b)
{
	int sum = 1;
	if(b == 0)
		return 1;
	for(int i = 0;i<b;i++)
		sum *= a;
	return sum;
}

int main()
{
	int n;
	scanf("%d",&n);
	int i = 1,j = 16;
	int mid;
	int ans;
	while(i<=j)
	{
		mid = (i+j)/2;
		if(power(2,mid)-1 >= n)
		{
			ans = mid;
			j = mid-1;
		}
		else
			i = mid+1;
	}
	printf("%d\n",ans);
}
