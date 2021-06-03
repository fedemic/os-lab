#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

double prod = 1;
int x;
pthread_mutex_t lock;

struct data {
	int index;
};


void * thread_func(void *arg) {

	struct data *mydata;	
	mydata = (struct data *)arg;

	double contr = pow(x, mydata->index);

	pthread_mutex_lock(&lock);
	printf("Contribution %d: %lf\t", mydata->index, contr);
	prod *= contr;
	printf("New product is: %lf\n", prod);
	pthread_mutex_unlock(&lock);

	pthread_exit(NULL);

}



int main() {

	int n, i;

	printf("Insert value of N: ");
	scanf("%d", &n);
	printf("Insert value of x: ");
	scanf("%d", &x);

	n++;
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
	printf("Final product is: %lf\n", prod);
	pthread_mutex_destroy(&lock);
}