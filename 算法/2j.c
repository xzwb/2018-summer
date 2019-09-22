#include <stdio.h>

int main()
{
	int n;
	int i,j;
	int temp;
	int star[101];
	int end[101];
	int cont = 1;
	while(~scanf("%d",&n))
	{ 
		cont = 1;	
		if(n == 0)
			return 0;
		for(i = 0; i < n; i++)
			scanf("%d %d",&star[i],&end[i]);
		for(i = 0; i < n-1; i++)
		{
			for(j = 0; j < n-i-1; j++)
			{
				if(end[j] > end[j+1])
				{
					temp = end[j];
					end[j] = end[j+1];
					end[j+1] = temp;
					temp = star[j];
					star[j] = star[j+1];
					star[j+1] = temp;
				}
			}
		}
		j = 0;
		for(i = 1; i < n; i++)
		{
			if(star[i] >= end[j] && end[i] > end[j])
			{
				j = i;
				cont++;
			}
		}
		printf("%d\n",cont);
	}
}
