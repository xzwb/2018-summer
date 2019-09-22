#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <sys/wait.h>

int biaozhi = 0;

struct rrr{
char a[256];
struct rrr *next;
};

char * get_exe_path( char *path ,char * buf, int count)
{
    int i;
    int rslt = readlink(path, buf, count - 1);
    if (rslt < 0 || (rslt >= count - 1))
    {
        return NULL;
    }
    buf[rslt] = '\0';
    for (i = rslt; i >= 0; i--)
    {
        if (buf[i] == '/')
        {
            buf[i + 1] = '\0';
            break;
        }
    }
    return buf;
}
int pdd=0;
//自定义的错误处理函数
void my_err(const char *err_string , int line)
{
	fprintf(stderr,"line:%d",line);
	perror(err_string);
	exit(1);
}

//打印文件列表的一行
int print_file(struct stat buf ,char *name)
{
	char buff[64];
	char hen[1024];
	struct passwd *pwd; //从该结构体获取文件所有者的名字
	struct group *grp; //从该结构体获取文件组名
	//判断文件类型
	if(S_ISLNK(buf.st_mode))	//判断是否为链接
		printf("l");
	if(S_ISREG(buf.st_mode))	//判断是否为一般文件
		printf("-");
	if(S_ISDIR(buf.st_mode))	//判断是否为目录
		printf("d");
	if(S_ISCHR(buf.st_mode))	//字符设备文件
		printf("c");
	if(S_ISBLK(buf.st_mode))	//块设备文件
		printf("b");
	if(S_ISFIFO(buf.st_mode))	//先进先出FIFO
		printf("f");
	if(S_ISSOCK(buf.st_mode))	//socket
		printf("s");
	
	//判断权限
	if((buf.st_mode & S_IRUSR))
		printf("r");
	else
		printf("-");

	if((buf.st_mode & S_IWUSR))
		printf("w");
	else
		printf("-");

	if((buf.st_mode & S_IXUSR))
		printf("x");
	else
		printf("-");
	
	if((buf.st_mode & S_IRGRP))
		printf("r");
	else
		printf("-");

	if((buf.st_mode & S_IWGRP))
		printf("w");
	else
		printf("-");

	if((buf.st_mode & S_IXGRP))
		printf("x");
	else
		printf("-");

	if((buf.st_mode & S_IROTH))
		printf("r");
	else
		printf("-");

	if((buf.st_mode & S_IWOTH))
		printf("w");
	else
		printf("-");

	if((buf.st_mode & S_IXOTH))
		printf("x");
	else
		printf("-");

	//打印链接数
	printf(" %4d ",buf.st_nlink);
	
	//打印文件用户和用户组
	pwd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);
	printf("%-8s",pwd->pw_name);
	printf("%-8s",grp->gr_name);
	
	//打印文件大小
	printf("%8d",buf.st_size);

	//打印文件最后一次修改的时间
	strcpy(buff,ctime(&buf.st_ctime));
	buff[strlen(buff)-1] = '\0';
	printf(" %s",buff);
	if(S_ISLNK(buf.st_mode))
	{
		printf(" %s->",name);
		printf("%s\n",get_exe_path(name,(char*)hen,1024));
	}
	else
		printf(" %s\n",name);
	return 0;
}

//只打印文件名的
void printf_name(char name[][256],int num)
{
	int i,j=0;
	for(i=0;i<num;i++)
	{
		j++;
		printf("%-20s",name[i]);
		if(j==5)
		{
			printf("\n");
			j = 0;
		}
	}
	printf("\n");
}

//从目录中获得文件列表 ls -a 和 ls
void have(char *path)
{
	DIR *dir;
	int i = 0;
	int number = 0;
	char a[10000][256];
	struct dirent *ptr;
	dir = opendir(path);
	if(dir == NULL)
		my_err("opendir",__LINE__);
	while((ptr = readdir(dir))!=NULL)
	{
		if(ptr < 0)
			continue;
		if(ptr->d_name[0] == '.' && biaozhi == 0);
			continue;
		strcpy(a[i],ptr->d_name);
		a[i][strlen(a[i])+1] = '\0';
		i++;
		number++;
	}
	printf_name(a,number);
}

//ls -l 和 ls -al
void have2(char *path)
{
	DIR *dir;
	int i = 0;
	int number = 0;
	DIR *dir2;
	struct stat buf;
	char a[10000][256];
	struct dirent *ptr;
	dir = opendir(path);
	char b[10];
	if(dir == NULL)
		my_err("opendir",__LINE__);
	chdir(path);
	while((ptr = readdir(dir))!=NULL)
	{
		if(strcmp(ptr->d_name,"4312") ==0)
			continue;
		if(opendir(ptr->d_name) < 0)
			continue;
		if(ptr->d_name[0] == '.' && biaozhi == 0)
			continue;
		if(lstat(ptr->d_name,&buf)<0)
			continue;
		strcpy(a[i],ptr->d_name);
		a[i][strlen(ptr->d_name)+1] = '\0';
		i++;
		number++;
	}
//	chdir(path);
	for(i=0;i<number;i++)
	{	
		if(lstat(a[i],&buf)<0)
			continue;
		if(S_ISDIR(buf.st_mode))
		{
			if((dir2 = opendir(a[i])) < 0)
				continue;
		}
		print_file(buf,a[i]);
	}
}

//实现ls -R
void R(struct rrr *head)
{
	DIR *dir;
	int k;
	int haha = 0;
	struct stat buf;
	char arr[256];
	struct dirent *ptr;
	struct rrr *p = head;
	struct rrr *p1,*phead = NULL,*p2;
	while(p != NULL)
	{
		printf("\n%s:\n",p->a);
		if((dir = opendir(p->a)) == NULL)
		{
			p = p->next;
			continue;
		}
	//	dir = opendir(p->a);
		while((ptr = readdir(dir)) != NULL)
		{
			if(strcmp(ptr->d_name,".")==0)
				continue;
			if(strcmp(ptr->d_name,"..") == 0)
				continue;
			if(ptr->d_name[0] == '.')
				continue;
			strcpy(arr,ptr->d_name);
			printf("%-15s",arr);
			haha += 1;
			if(haha == 4)
			{
				printf("\n");
				haha = 0;
			}
			sprintf(arr,"%s%s",p->a,ptr->d_name);
			lstat(arr,&buf);
			if(S_ISDIR(buf.st_mode))
			{
				k = strlen(arr);
				arr[k] = '/';
				arr[k+1] = '\0';
				p1 = (struct rrr*)malloc(sizeof(struct rrr));
				strcpy(p1->a,arr);
				if(phead == NULL)
					phead = p1;
				else
					p2->next = p1;
				p2 = p1;
			}
		}
		closedir(dir);
		if(phead == NULL)
		{
			p = p->next;
			continue;
		}
		p2->next = NULL;
		printf("\n");
		R(phead);
		free(phead);
		phead = NULL;
		p = p->next;
	}
}

//实现ls -lR
void LR(struct rrr *head)
{
	DIR *dir;
	int k;
	DIR *dir2;
	struct passwd *pwd; //从该结构体获取文件所有者的名字
	struct group *grp; //从该结构体获取文件组名
	struct stat buf;
	char arr[256];
	struct dirent *ptr;
	struct rrr *p = head;
	struct rrr *p1,*phead = NULL,*p2;
	while(p != NULL)
	{
		printf("\n%s:\n",p->a);
		if((dir = opendir(p->a)) == NULL)
		{
			p = p->next;
			continue;
		}
		while((ptr = readdir(dir)) != NULL)
		{
			if(strcmp(ptr->d_name,".")==0)
				continue;
			if(strcmp(ptr->d_name,"..") == 0)
				continue;
			if(strcmp(ptr->d_name,"proc")==0)
				continue;
			if(strcmp(ptr->d_name,"mimetypes") == 0)
				continue;
			if(ptr->d_name[0] == '.')
				continue;
			if(strcmp(ptr->d_name,"OS")==0)
				continue;
			strcpy(arr,ptr->d_name);
			sprintf(arr,"%s%s",p->a,ptr->d_name);
			if(lstat(arr,&buf) < 0)
				continue;
			if(buf.st_uid != 0 && buf.st_uid != 1000)
				continue;
			pwd = getpwuid(buf.st_uid);
			if(strcmp(pwd->pw_name,"root")!=0 && strcmp(pwd->pw_name,"xzwb")!=0)
				continue;
			print_file(buf,arr);
			if(S_ISDIR(buf.st_mode))
			{
				k = strlen(arr);
				arr[k] = '/';
				arr[k+1] = '\0';
				p1 = (struct rrr*)malloc(sizeof(struct rrr));
				strcpy(p1->a,arr);
				if(phead == NULL)
					phead = p1;
				else
					p2->next = p1;
				p2 = p1;
			}
		}
		closedir(dir);
		if(phead == NULL)
		{
			p = p->next;
			continue;
		}
		p2->next = NULL;
		printf("\n");
		LR(phead);
		free(phead);
		phead = NULL;
		p = p->next;
	}
}

int main(int argc,char *argv[])
{
	char path[32];
	struct rrr *head,*p;
	int choose=0;
	int i,j,k,m=0,num=0,c;
	struct stat buf;
	char *hen[256];
	int pid;
	if(argc==1)
 	{
		strcpy(path,"./");
		have(path);
		return 0;
	}
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] == '-')
		{
			k = strlen(argv[i]);
			for(j=0;j<k;j++)
			{
				if(argv[i][j] == 'a')
					choose += 1;
				if(argv[i][j] == 'l')
					choose += 2;
				if(argv[i][j] == 'R')
					choose += 4;
			}
		}
		else
		{
			strcpy(path,argv[i]);
			num++;
		}
	}
	i--;
	if(num != 0)
	{
		if(stat(path,&buf) == -1)
			my_err("path",__LINE__);
		if(S_ISDIR(buf.st_mode))
		{
			p = (struct rrr*)malloc(sizeof(struct rrr));
			head = p;
			strcpy(p->a,path);
			p->next = NULL;
			if(path[strlen(argv[i])-1] != '/')
			{
				path[strlen(argv[i])] = '/';
				path[strlen(argv[i])+1] = '\0';
			}	
			switch(choose)
			{
				case 1:
					{
						biaozhi = 1;
						have(path);
						break;
					}
				case 2:
					{
						biaozhi = 0;
						have2(path);
						break;
					}
				case 4:
					{
						R(head);
						break;
					}
				case 3:
					{
						biaozhi = 1;
						have2(path);
						break;
					}
				case 6:
					{
						LR(head);
						break;
					}
				case 7:
					{
						LR(head);
						break;
					}
				case 5:
					{
						R(head);
						break;
					}
				case 0:
					{
						biaozhi = 0;
					       have(path);
					       break;
				        }
				default:break;
			}
		}
		else
		{
			lstat(path,&buf);
			switch(choose)
			{
				case 1:
					{
						printf("%s\n",path);
						break;
					}
				case 2:
					{
						print_file(buf,path);
						break;
					}
				case 4:
					{
						printf("%s\n",path);
						break;
					}
				case 3:
					{
						print_file(buf,path);
						break;
					}
				case 6:
					{
						print_file(buf,path);
						break;
				        }
				case 7:
					{
						print_file(buf,path);
						break;
					}
				case 5:
					{
						printf("%s\n",path);
						break;
					}
				default:break;
			}
		}
	}
	else
	{
		p = (struct rrr*)malloc(sizeof(struct rrr));
		head = p;
		p->next = NULL;
		strcpy(p->a,"./");
		strcpy(path,"./");
		switch(choose)
		{
			case 1:
				{
					biaozhi = 0;
					have(path);
					break;
				}
			case 2:
				{
					biaozhi = 1;
					have2(path);
					break;
				}
			case 4:
				{
					R(head);
					break;
				}
			case 6:
				{
					LR(head);
					break;
				}
			case 3:
				{
					biaozhi = 1;
					have2(path);
					break;
				}
			case 7:
				{
					LR(head);
					break;
				}
			case 5:
				{
					R(head);
					break;
				}
			default:break;
		}	
	}
	return 0;
}

