#include <stdio.h>

int main()
{
	int t;
	int n;
	int i,j,k2=0,k1=0;
	int cont;
	int b;
	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		k1 = 0;
		k2 = 0;
		cont = 0;
		scanf("%d",&n);
		for(j=0;j<n;j++)
		{
			scanf("%d",&b);
			if(b%3==0)
				cont++;	
			else
			{
				if(b%3 == 2)
					k2++;
				else if(b%3 == 1)
					k1++;
			}
		}	
		if(k2>=k1 && k1!=0)
		{
			cont += k1;
			k2 = k2-k1;
			cont += k2/3;
		}
		else if(k2>=k1 && k1==0)
			cont += k2/3;
		else if(k1>k2)
		{
			cont += k2;
			k1 = k1-k2;
			cont += k1/3;
		}
		printf("%d\n",cont);

	}
}
