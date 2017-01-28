#include <pthread.h>
#include <stdio.h>
//#define NUM_THREADS 5

static void *PrintHello(void *threadid)
{
	long tid;
	tid = (long)threadid;
	printf("Hello World! It’s me, thread # %ld!\n", tid);
	pthread_exit(NULL);
}

static void *PrintBye(void *threadid)
{
	long tid;
	tid = (long)threadid;
	printf("Bye Bye World! It’s me, thread # %ld!\n", tid);
	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
		//pthread_t threads[NUM_THREADS];
			pthread_t hiloHello;
			pthread_t hiloBye;
		int rc;
		long t1=1;
		long t2=2;
		//for(t=0; t<NUM_THREADS; t++){
		printf("In main: creating thread hello\n");
		rc = pthread_create(&hiloHello, NULL, PrintHello, (void *)t1);
			if (rc){
					printf("ERROR; return code from pthread_create() is %d\n", rc);
					exit(-1);
			}
		rc = pthread_create(&hiloBye, NULL, PrintBye, (void *)t2);
			if (rc){
					printf("ERROR; return code from pthread_create() is %d\n", rc);
					exit(-1);
			}	

	pthread_join(hiloHello,NULL);
	pthread_join(hiloBye,NULL);
}
	

