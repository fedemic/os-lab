#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

double prod = 1;
int x;
pthread_mutex_t lock;


void * thread_func(void *i) {

	int th_i = *(int *)i;
	double contr = pow(x, th_i);

	pthread_mutex_lock(&lock);
	printf("Contribution %d: %lf\t", th_i, contr);
	prod *= contr;
	printf("New product is: %lf\n", prod);
	pthread_mutex_unlock(&lock);

	pthread_exit(NULL);

}



int main() {

	int n;

	printf("Insert value of N: ");
	scanf("%d", &n);
	printf("Insert value of x: ");
	scanf("%d", &x);

	n++;
	pthread_t threads[n];

	pthread_mutex_init(&lock, NULL);

	for(int i=0; i<n; i++) {
		pthread_create(&threads[i], NULL, thread_func, &i);
	}

	for(int i=0; i<n; i++) {
		pthread_join(&threads[i], NULL);
	}

	sleep(1);
	printf("Final product is: %lf\n", prod);
	pthread_mutex_destroy(&lock);
}