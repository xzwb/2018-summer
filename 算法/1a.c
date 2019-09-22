#include <stdio.h>

int main()
{
	int cont;
	int n;
	int i;
  	long long q;
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		cont = 0;
		scanf("%lld",&q);
		if(q<1)
		{
			printf("-1\n");
			continue;
		}
		if(q==1)
		{
			printf("0\n");
			continue;
		}
		while(q!=1 && q>1)
		{
			if(q%5 == 0)
			{
				q = 4*q/5;
				cont++;
				continue;
			}
			if(q%3 == 0)
			{
				q = 2*q/3;
				cont++;
				continue;
			}
			if(q%2 == 0)
			{
				q = q/2;
				cont++;
			}
			else
			{
				printf("-1\n");
				break;
			}
		}
		if(q == 1)
			printf("%d\n",cont);

	}
	return 0;
}

