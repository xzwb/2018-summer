#include <stdio.h>

int main()
{
	double n,m;
	int i;
	double min = 0;
	double x,y;
	scanf("%lf %lf",&n,&m);
	for(i=0;i<n;i++)
	{
		scanf("%lf %lf",&x,&y);
		if(min == 0)
			min = x/y;
		if(x/y < min)
			min = x/y;
	}
	printf("%lf",min*m);
}
