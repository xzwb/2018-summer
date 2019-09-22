#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int main()
{
	int n,m;
	int i;
	int a[101];
	int max = 0;
	scanf("%d %d",&n,&m);
	for(i = 0;i<n;i++)
		scanf("%d",&a[i]);
	sort(a,a+n);
	for(i=0;i<m;i++)
	{
		if(a[i] > 0)
			break;
		else
		{
			if(a[i] <= 0)
				max -= a[i];
		}
	}
	printf("%d\n",max);

}
