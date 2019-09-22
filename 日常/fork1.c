#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int x = 1;

int main()
{
	x++;
	if(fork() == 0)
		printf("p1: x = %d\n",++x);
	sleep(1);
	printf("p2: x = %d\n",x);
	exit(0);
}
