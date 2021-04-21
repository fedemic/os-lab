#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define N_TRHREADS 2

void *thread_func(void *id) {

	int th_id = *(int *)id;				// get indexing value
	
	printf("Thread %d processing\n", th_id);		
	sleep(1);
	printf("Thread %d terminating\n", th_id);
}

int main() {

	pthread_t threads[N_TRHREADS];
	int i;

	for(i=0; i<N_TRHREADS; i++) {				// generate N_THREADS
		
		printf("MAIN: Thread %d creation...\n", i);				// string printed from main
		if(pthread_create(&threads[i], NULL, thread_func, &i) != 0) {		// thread generation and error checking
			printf("Error creating thread %d.\n", i);
			exit(1);
		}
		sleep(3);
	}

	return 0;
}