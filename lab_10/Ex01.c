#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

double sum = 0;
pthread_mutex_t lock;


struct data {
	int index;
};

void * thread_func(void *arg) {

	struct data *mydata;	
	mydata = (struct data *)arg;

	double term = 1.0/pow(2, mydata->index);

	pthread_mutex_lock(&lock);
	sum += term;
	printf("New sum is: %lf\n", sum);
	pthread_mutex_unlock(&lock);

	pthread_exit(NULL);

}



int main() {

	int n, i;

	printf("Insert value of N: ");
	scanf("%d", &n);

	pthread_t threads[n];
	struct data myparam[n];

	pthread_mutex_init(&lock, NULL);

	for(i=0; i<n; i++) {
		myparam[i].index = i;
		pthread_create(&threads[i], NULL, thread_func, (void *)&myparam[i]);
	}

	for(i=0; i<n; i++) {
		pthread_join(&threads[i], NULL);
	}

	sleep(1);
	printf("Final sum is: %lf\n", sum);
	pthread_mutex_destroy(&lock);
}