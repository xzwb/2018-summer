#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	if((pid =fork())==0)
	{
		printf("a");fflush(stdout);
	}
	else
	{
		printf("b");fflush(stdout);
		waitpid(-1,NULL,0);
	}
	if(pid == 0)
	{	printf("c");fflush(stdout);}
	else
	{	printf("d");fflush(stdout);}
	exit(0);
}
