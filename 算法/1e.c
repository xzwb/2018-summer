#include <stdio.h>

int main()
{
	int n,m;
	int q;
	int i;
	scanf("%d",&q);
	for(i=0;i<q;i++)
	{
		scanf("%d %d",&n,&m);
		if(n == m+1)
		{
			printf("second\n");
			continue;
		}
		n = n%(m+1);
		if((m - n) >= 0 && (m-n) <= m && n!=0)
			printf("first\n");
		else
			printf("second\n");
	}
}
