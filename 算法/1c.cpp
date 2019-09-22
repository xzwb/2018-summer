#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;


int a[1000000];

int main()
{
	int n;
	int cont=0;
	int i = 0,j;
	scanf("%d",&n);
	for(i=0;i<n;i++)
		scanf("%d",&a[i]);
	sort(a,a+n);
	j = 1;
	for(i=0;i<n;i++)
	{
		if(a[i]>=j)
		{
			cont++;
			j++;
		}
	}
	printf("%d",cont);
}
