#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 2

sem_t sem1, sem2;		// two semaphores to handle correct order

void * thread_func1() {

	sem_wait(&sem1);		// wait sem 1
	printf("I am thread with ID: %lu\n", pthread_self());			// CS
	sem_post(&sem2);		// post sem 2 to let second thread to enter CS
}

void * thread_func2() {

	sem_wait(&sem2);		// wait sem 2
	printf("I am thread with ID: %lu\n", pthread_self());			// CS
	sem_post(&sem1);		// post sem 1
}


int main() {

	pthread_t threads[N];
	int i;

	sem_init(&sem1, 0, 1);		// first thread will enter first
	sem_init(&sem2, 0, 0);		// second thread will wait

	if(pthread_create(&threads[1], NULL, thread_func1, NULL) != 0) {
		printf("Error generating thread %d. Exiting...\n", 1);
	}
	if(pthread_create(&threads[2], NULL, thread_func2, NULL) != 0) {
		printf("Error generating thread %d. Exiting...\n", 2);
	}


	for(i=0; i<N; i++) {
		pthread_join(threads[i], NULL);
	}

	sem_destroy(&sem1);
	sem_destroy(&sem2);

	return 0;

}
