#include <stdio.h>
#include <string.h>

int main()
{
	int i, j, k;
	int n, m;
	int temp;
	int sum = 0;
	int day[1001];
	int score[1001];
	int judge[1005] = {0};
	scanf("%d", &n);
	for (i = 0; i < n; i++){
		sum = 0;
		memset(judge, 0, sizeof(judge));
		scanf("%d", &m);
		for (j = 0; j < m; j++){
			scanf("%d", &day[j]);
		}
		for (j = 0; j < m; j++){
			scanf("%d", &score[j]);
		}
		for (k = 0; k < m-1; k++){
			for (j = 0; j < m-k-1; j++){
				if(score[j] < score[j+1]){
					temp = score[j];
					score[j] = score[j+1];
					score[j+1] = temp;
					temp = day[j];
					day[j] = day[j+1];
					day[j+1] = temp;
				}
			}
		}
		for (j = 0; j < m; j++){
			if(judge[day[j]] == 0){
				judge[day[j]] = 1;
				continue;
			}
			else{
				while(--day[j]){
					if(judge[day[j]] == 0){
						judge[day[j]] = 1;
						break;
					}
				}
				if(day[j] == 0){
					sum += score[j];
				}
			}
		}
		printf("%d\n", sum);
	}

}
