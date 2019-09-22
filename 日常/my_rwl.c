#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

//自定义的错误处理函数
void my_err(const char *str , int line)
{
	fprintf(stderr,"line:%d",line);
	perror(str);
	exit(1);
}

//数据读取函数
int my_read(int fd)
{
	int len;//获取文件长度
	char read_buf[3000]; 
	int i;
	int ret;

	if(lseek(fd,0,SEEK_END) == -1)
		my_err("lseek",__LINE__);
	if((len = lseek(fd,0,SEEK_CUR)) == -1)
		my_err("lseek",__LINE__);
	if(lseek(fd,0,SEEK_SET) == -1)
		my_err("lseek",__LINE__);

	printf("len:%d\n",len);

	//读数据
	if((ret = read(fd,read_buf,len)) < 0)
		my_err("read",__LINE__);

	//打印数据
	for(i=0;i<len;i++)
		printf("%c",read_buf[i]);
	printf("\n");
	return ret;
}

int main()
{
	int fd;
	if((fd = open("../Plan/文件操作.txt",O_RDONLY)) < 0)
		my_err("open",__LINE__);
	my_read(fd);
	return 0;
}

