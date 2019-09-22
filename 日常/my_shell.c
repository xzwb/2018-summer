#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

struct dd{
	char haha[2][256];
	int ha;
}cd;

struct history{
	char a[100];
	struct history *next;
};

//打印提示符
void print_prompt()
{
	struct passwd *usrname;
	usrname = getpwuid(getuid());
	char buf[256];
	memset(buf,0,sizeof(buf));
	printf("my_shell:%s",getcwd(buf,256));
	if(strcmp(usrname->pw_name,"root")==0)
		printf("#");
	else
		printf("$");
}

//获得命令行输入
void get_input(char *buf)
{
	int len = 0;
	char ch;
	ch = getchar();
	while(len < 256 && ch != '\n')
	{
		buf[len++] = ch;
		ch = getchar();
	}
	if(len == 256)
	{
		printf("command is too long\n");
		exit(1);
	}
	buf[len] = '\n';
	buf[++len] = '\0';
}

//解析命令行
void explain_input(char *buf , int *cont , char list[100][256]) 
{
	int i=0;
	*cont = 0;
	int j;
	while(1)
	{
		if(buf[i] == ' ')
		{
			i++;
			continue;
		}
		if(buf[i] == '\n')
			break;
		else
		{
			j = 0;
			while(buf[i] != ' ' && buf[i] != '\n')
			{
				list[*cont][j] = buf[i];
				j++;
				i++;
			}	
			list[*cont][j] = '\0';
			*cont += 1;
		}
	}
}

//内置命令直接执行
void in_cd(char list[100][256],int cont)
{
	if(strcmp(list[0],"cd") != 0)
		return;
	char buf[256];
	struct passwd *usrname;
	usrname = getpwuid(getuid());
	getcwd(cd.haha[cd.ha],sizeof(cd.haha[cd.ha]));
	if(cd.ha == 0)
		cd.ha++;
	else
		cd.ha--;
	if(strcmp(list[1],"~")==0)
	{
		if(strcmp(usrname->pw_name,"xzwb")==0)
			strcpy(buf,"/home/xzwb/");
		else if(strcmp(usrname->pw_name,"root")==0)
			strcpy(buf,"/root/");	
	}
	else if(strcmp(list[1],"-")==0)
		strcpy(buf,cd.haha[cd.ha]);
	else
		strcpy(buf,list[1]);
	chdir(buf);
}

//实现除了cd以外的外部命令
void in_command(char list[100][256] , int cont)
{
	int background = 0; //标识是否有后台运行符&
	int how = 0; //用于标识是否有><|
	pid_t pid;
	int i;
	int fd;
	int he;
	int statu;
	char *arg[256];
	char *arg2[256];
	char *file;
	for(i=0;i<cont;i++)
		arg[i] = (char*)list[i];
	arg[cont] = NULL;
	for(i=0;i<cont;i++)
	{
		if(strcmp(arg[i],">")==0)
		{
			file = arg[i+1];
			how += 1;
			he = i;
		}
		if(strcmp(arg[i],"<")==0)
		{
			file = arg[i+1];
			how += 2;
			he = i;
		}
		if(strcmp(arg[i],">>")==0)
		{
			file = arg[i+1];
			how += 11;
			he = i;
		}
		if(strcmp(arg[i],"<<")==0)
		{
			file = arg[i+1];
			how += 22;
			he = i;
		}
		if(strcmp(arg[i],"|")==0)
		{
			int c = 0;
			for(int j = i+1 ; j < cont;j++,c++)
				arg2[c] = arg[j];
			arg2[c] = NULL;
			how += 4;
			he = i;
		}
		if(strcmp(arg[i],"&")==0)
		{
			arg[i] = NULL;
			background += 1;
		}
	}
	if(how!=0)
		arg[he] = NULL;
	if(background == 1 && strcmp(list[cont-1],"&") != 0)
		background--;
	if((pid = fork())<0)
		perror("fork error\n");
	if(pid == 0)
	{
		switch(how)
		{
			case 0:
				{
					execvp(arg[0],arg);
					exit(0);
					break;
				}
			case 1:
				{
					fd = open(file,O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
					dup2(fd,1);
					execvp(arg[0],arg);
					exit(0);
					break;
				}
			case 2:
				{
					if((fd = open(file,O_RDONLY,0644)) < 0)
						printf("openfile falsh\n");
					close(0);
					dup2(fd,0);
					execvp(arg[0],arg);
					exit(0);
					break;
				}
			case 11:
				{
					fd = open(file,O_RDWR|O_CREAT|O_APPEND,S_IRWXU);
					dup2(fd,1);
					execvp(arg[0],arg);
					exit(0);
					break;
				}
			case 22:
				{
					if((fd = open(file,O_RDONLY)) < 0)
						perror("open");
					dup2(fd,0);
					execvp(arg[0],arg);
					exit(0);
					break;
				}
			case 4:
				{
					int pid2;
					int satu;
					int fd2;
					if((pid2 = fork()) < 0)
						perror("fork error");
					if(pid2 == 0)
					{
						if((fd2 = open("/tmp/1.txt",O_WRONLY|O_CREAT|O_TRUNC,0644)) < 0)
							perror("open");
						dup2(fd2,1);
						execvp(arg[0],arg);
						exit(0);
					}
					waitpid(pid2,&satu,0);
					close(fd2);
					fd2 = open("/tmp/1.txt",O_RDONLY);
					dup2(fd2,0);
					execvp(arg2[0],arg2);
					if(remove("/tmp/1.txt") < 0)
						perror("remove error");
					exit(0);
					break;
				}
			default:printf("输入错误!\n");
		}
	}
	if(background == 1)
		return;
	if(background == 0)
		waitpid(pid,&statu,0);
}

static void mask_ctrl_c(int signo)
{
	printf("\n");
	print_prompt();
	fflush(stdout);
}                         

void history_ai(char list[100][256])
{
	if(strcmp(list[0],"history")==0)
	{
		read_history(NULL);
	}
}

int main()
{
	cd.ha = 0;
	int cont;
	int len;
	char list[100][256];
	signal(SIGINT,mask_ctrl_c);
	while(1)
	{
		char *buf = (char*)malloc(sizeof(256));
		memset(list,0,sizeof(list));
		memset(buf,0,sizeof(buf));
		print_prompt();
		buf = readline("myshell&");
		add_history(buf);
		write_history(NULL);
		buf[strlen(buf)] = '\n';		
		if(strcmp(buf,"exit\n") == 0)
			exit(0);
		explain_input(buf,&cont,list);
		in_cd(list,cont);
		history_ai(list);
		in_command(list,cont);
		free(buf);
	}
}
