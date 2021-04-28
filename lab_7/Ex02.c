#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

long unsigned num;
int divisible=0, prime=0;

struct data {
	int start;
	int stop;
};


void *thread_func(void * thread_arg) {
	struct data *mydata;
	mydata = (struct data *)thread_arg;

	int i;

	for (i=mydata->start; i<mydata->stop; i++) {
		if((num % i) == 0) {
			divisible = i;
			pthread_exit(NULL);
		}
	}

	prime++;
	pthread_exit(NULL);
}


int main() {

	int n_threads, i;

	do {
		printf("Insert a positive number N: ");
		scanf("%lu", &num);
	} while(num <= 0);

	do {
		printf("Insert number of threads: ");
		scanf("%d", &n_threads);
	} while(n_threads <= 0);

	pthread_t threads[n_threads];
	struct data param[n_threads];
	int start=2, stop, tests, step;

	stop = ceil(num/2);
	tests = stop-start;
	step = ceil(tests/n_threads);

	clock_t begin_exec = clock();

	for(i=0; i<n_threads; i++) {

		param[i].start = start;
		param[i].stop = start + step;

		if (pthread_create(&threads[i], NULL, thread_func, (void *)&param[i]) != 0) {
			printf("Error generating thread %d. Exiting program...", i);
			exit(1);
		}

		start += step;
	} 

	while(divisible == 0 && prime != n_threads);
	
	if(divisible != 0) {
		printf("Number %lu is divisible by: %d\n", num, divisible);
	}
	else if(prime == n_threads) {
		printf("Number %lu is prime\n", num);
	}

	clock_t end_exec = clock();
	double exec_time = (double)(end_exec - begin_exec) / CLOCKS_PER_SEC;
	printf("Execution time: %lf\n", exec_time);

	return 0;
}