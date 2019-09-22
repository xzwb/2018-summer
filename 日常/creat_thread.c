#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int i;

int *thread(void *arg)
{
	pthread_detach(pthread_self());
	pthread_t newthid;
	i++;
	newthid = pthread_self(); //返回线程自身id
	printf("this is a new thread , thread id = %u\n",newthid);
	printf("i = %d\n",i);
	sleep(5);
	return NULL;
}

int main()
{
	pthread_t thid,th;
	while(1)
	{
	scanf("%d",&i);
	printf("main thread id is %u\n",pthread_self()); //打印线程主id
	i = 100;
	printf("i = %d\n",i);
	if(pthread_create(&thid , NULL , (void*)thread , NULL) != 0)
	{
		printf("thread creation failed\n");
		exit(1);
	}
//	pthread_join(thid,NULL);
	printf("haha\nui =%d",i);
	}
	exit(0);
}

