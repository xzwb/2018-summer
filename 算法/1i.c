#include <stdio.h>
#include <string.h>
int left[1000000];
int left_len;
	int left_count;
	int left_sum;
	int right[10000000];
	int right_len;
	int right_count;
	int right_sum;
	char ch[10];


int main()
{
	int n,c,m,l,j,temp,i;
	scanf("%d",&c);
	for(i=0;i<c;i++)
	{
		left_len = 0;
		right_len = 0;
		right_count = 0;
		left_count = 0;
		right_sum = 0;
		left_sum = 0;
		scanf("%d %d",&l,&m);
		l = l*100;
		for(j=0;j<m;j++)
		{
			scanf("%d %s",&temp,ch);
			if(strcmp(ch,"left") == 0)
			{
				left[left_len] = temp;
				left_len++;
			}
			else if(strcmp(ch,"right") == 0)
			{
				right[right_len] = temp;
				right_len++;
			}
		}
		for(j=0;j<left_len;j++)
		{
			left_sum += left[j];
			if(left_sum > l)
			{
				left_sum = 0;
				j--;
				left_count++;
			}
		}
		if(left_sum<=l && left_len!=0)
			left_count++;

		for(j=0;j<right_len;j++)
		{
			right_sum += right[j];
			if(right_sum > l)
			{
				right_sum = 0;
				j--;
				right_count++;
			}
		}
		if(right_sum<=l && right_len != 0)
			right_count++;

		if(left_count > right_count)
			printf("%d\n",left_count*2-1);
		else 
			printf("%d\n",2*right_count);

	}
}
