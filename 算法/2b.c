#include <stdio.h>

int main()
{
	int n;
	scanf("%d",&n);
	int i;
	int cont;
	if(n == 1)
	{
		printf("1\n");
		return 0;
	}
	cont = n/2 + 1;
	printf("%d\n",cont);
}
