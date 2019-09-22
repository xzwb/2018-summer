#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

pthread_key_t key;
int tsd = 0;

void *thread2(void *arg)
{
	tsd = 10;
	printf("thread %d is running\n",pthread_self());
	pthread_setspecific(key,(void *)tsd);
	printf("thread %d returns %d\n",pthread_self(),pthread_getspecific(key));
}

void *thread1(void *arg)
{
	tsd = 5;
	pthread_t thid2;
	printf("1tsd = %d\n",tsd);
	printf("thread %d is running\n",pthread_self());
	pthread_setspecific(key,(void*)tsd);
	pthread_create(&thid2,NULL,thread2,NULL);
	pthread_join(thid2,NULL);
	printf("thread %d return %d\n",pthread_self(),pthread_getspecific(key));
	printf("1return tsd = %d\n",tsd);
}

int main()
{
	pthread_t thid1;
	printf("main thread begins running\n");
	pthread_key_create(&key,NULL);
	pthread_create(&thid1,NULL,thread1,NULL);
	pthread_join(thid1,NULL);
	printf("main tsd = %d\n",tsd);
	pthread_key_delete(key);
	printf("main thread exit\n");
	return 0;
}
