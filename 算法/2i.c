#include <stdio.h>

int main()
{
	int n, m;
	int i, j, k;
	int temp;
	int sum;
	int star[201];
	int end[201];
	scanf("%d", &n);
	for (i = 0; i < n; i++){
		sum = 10;
		scanf("%d", &m);
		for (j = 0; j < m; j++){
			scanf("%d %d", &star[j], &end[j]);
			if(star[j] > end[j]){
				temp = star[j];
				star[j] = end[j];
				end[j] = temp;
			}
		}
		for (k = 0; k < m-1; k++){
			for (j = 0; j < m-k-1; j++){
				if(end[j] > end[j+1]){
					temp = end[j];
					end[j] = end[j+1];
					end[j+1] = temp;
					temp = star[j];
					star[j] = star[j+1];
					star[j+1] = temp;
				}
			}
		}
		k = 0;
		for (j = 1; j < m; j++){
			if(end[k]%2 == 0){
				if(end[j] > end[k]){
					if(star[j] > end[k]){
						k = j;
						continue;
					}
					else{
						sum += 10;
						continue;
					}
				}
				else{
					sum += 10;
					continue;
				}
			}
			else{
				if(end[j] > end[k]){
					if(star[j] > end[k]+1){
						k = j;
						continue;
					}
					else{
						sum += 10;
						continue;
					}
				}
				else{
					sum += 10;
					continue;
				}
			}
		}
		printf("%d\n", sum);
	}
}

