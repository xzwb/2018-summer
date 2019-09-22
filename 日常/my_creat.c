#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc , char *argv[])
{
	int fd; //文件标识符

	if((fd = open(argv[1],O_CREAT|O_EXCL,S_IRUSR|S_IWUSR)) == -1)
	{
		perror("open");
		exit(1);
	}
	else
		printf("open success!\n");
	close(fd);
	return 0;

}
