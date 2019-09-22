#include <stdio.h>

int main()
{
	int a[101]={0};
	char b[101];
	int n,d;
	int i,j;
	int max=0,cont=0;
	scanf("%d %d",&n,&d);
	for(i=0;i<d;i++)
	{
		scanf("%s",b);
		getchar();
		for(j=0;j<n;j++)
			a[i] += (b[j]-48);
	}
	for(i=0;i<d;i++)
	{
		if(a[i]!=n)
			cont++;
		else //if(max < cont)
		{
			if(max < cont)
				max = cont;
			cont = 0;
		}
	}
	if(max < cont)
		max = cont;
	printf("%d\n",max);
}
