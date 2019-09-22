#include <stdio.h>

int main()
{
	int k;
	int i;
	char ch[20];
	int eight,six;
	scanf("%d",&k);
	if(k > 36)
	{
		printf("-1\n");
		return 0;
	}
	six = k%2;
	eight = k/2;
	int cont = 0;
	for(i = 0; i < eight; i++)
	{
		ch[cont] = '8';
		cont++;	
	}
	for(i = 0; i < six; i++)
	{
		ch[cont] = '6';
		cont++;
	}
	ch[cont] = '\0';
	printf("%s\n",ch);
}
