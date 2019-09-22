//获取文件属性
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc , char *argv[])
{
	struct stat buf;
	if(argc != 2)
	{
		printf("./a.out <file_name>\n");
		exit(0);
	}
	if(stat(argv[1],&buf) < 0)
	{
		perror("stat");
		exit(1);
	}

	printf("time : %s",ctime(&buf.st_ctime));
	return 0;
}
