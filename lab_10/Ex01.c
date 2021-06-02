#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

double sum = 0;
pthread_mutex_t lock;


void * thread_func(void *i) {

	int th_i = *(int *)i;	
	double term = 1.0/pow(2, th_i);

	pthread_mutex_lock(&lock);
	sum += term;
	printf("New sum is: %lf\n", sum);
	pthread_mutex_unlock(&lock);

	pthread_exit(NULL);

}



int main() {

	int n;

	printf("Insert value of N: ");
	scanf("%d", &n);

	pthread_t threads[n];

	pthread_mutex_init(&lock, NULL);

	for(int i=0; i<n; i++) {
		pthread_create(&threads[i], NULL, thread_func, &i);
	}

	for(int i=0; i<n; i++) {
		pthread_join(&threads[i], NULL);
	}

	sleep(1);
	printf("Final sum is: %lf\n", sum);
	pthread_mutex_destroy(&lock);
}