#include <stdio.h>

int main()
{
	float m,n;
	int i,j;
	float sum;
	float temp;
	float k[1001] , f[1001];
	double a[1001];
	while(1)
	{
		scanf("%f %f", &m, &n);
		sum = 0;
		if(m == -1 && n == -1)
			return 0;
		for(i = 0; i < n; i++)
		{
			scanf("%f %f",&k[i],&f[i]);
			a[i] = k[i]/f[i];
		}
		for(i = 0; i < n-1; i++)
		{
			for(j = 0; j < n-i-1; j++)
			{
				if(a[j] < a[j+1])
				{
					temp = a[j];
					a[j] = a[j+1];
					a[j+1] = temp;
					temp = f[j];
					f[j] = f[j+1];
					f[j+1] = temp;
					temp = k[j];
					k[j] = k[j+1];
					k[j+1] = temp;
				}
			}
		}
		for(i = 0; i < n; i++)
		{
			if(m >= f[i])
			{
				sum += k[i];
				m -= f[i];
			}
			else
			{
				float hen = m/f[i];
				sum += hen * k[i];
				break;
			}
		}
		printf("%.3f\n",sum);
	}
}
