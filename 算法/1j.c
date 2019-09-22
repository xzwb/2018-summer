#include <stdio.h>

int main()
{
	int n;
	int i;
	int left,right;
	int last;
	int count = 1;
	int a[1000000];
	char ch[1000000];
	scanf("%d",&n);
	left = 0;
	right = n-1;
	for(i=0;i<n;i++)
		scanf("%d",&a[i]);
	if(n == 1)
	{
		printf("1\nL\n");
		return 0;
	}
	if(a[left]<a[right])
	{
		last = left;
		left++;
		ch[0] = 'L';
	}
	else
	{
		last = right;
		right--;
		ch[0] = 'R';
	}
	for(i=1;i<n-1;i++)
	{
		if(a[left] < a[last] && a[right] < a[last])
			break;
		if(a[left] < a[right] && a[left] > a[last])
		{
			last = left;
			left++;
			ch[i] = 'L';
		}
		else
		{
			if(a[right] < a[last] && a[left]>a[last])
			{
				last = left;
				left++;
				ch[i] = 'L';
				count++;
				continue;
			}
			last = right;
			right--;
			ch[i] = 'R';
		}
		count++;
	}
	if(i==n-1 && a[left] > a[last])
		ch[count++] = 'L';
	printf("%d\n",count);
	for(i=0;i<count;i++)
		printf("%c",ch[i]);
	printf("\n");
}
