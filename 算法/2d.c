#include <stdio.h>

int main()
{
	int t[1001];
	int e;
	int a,b,c,n;
	int i;
	int sum = 0;
	scanf("%d %d %d %d %d",&n,&a,&b,&c,&e);
	for(i=0;i<n;i++)
		scanf("%d",&t[i]);
	if(b>=c)
		printf("%d\n",a*n);
	else
	{
		for(i=0;i<n;i++)
			sum += (a-(e-t[i])*b) + c*(e-t[i]);
		printf("%d\n",sum);
	}
}
