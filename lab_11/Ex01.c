#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

long unsigned num;					// global variables
int divisible=0, prime=0;

pthread_mutex_t lock;			// mutex lock
pthread_cond_t  cond;			// condition variable

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

			pthread_mutex_lock(&lock);		// acquire lock for CS
			// CS begin
			divisible = i;
			// CS end
			pthread_mutex_unlock(&lock);	// release lock after CS
			pthread_cond_signal(&cond);		// signal condition variable after finding a divisor
			pthread_exit(NULL);
		}
	}

	pthread_mutex_lock(&lock);			// acquire lock for CS
	// CS begin
	prime++;
	// CS end
	pthread_mutex_unlock(&lock);		// release lock after CS
	pthread_cond_signal(&cond);			// signal cond variable if no divisor found

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
	} while(n_threads <= 0 || n_threads >= num/2);

	// initialization
	pthread_mutex_init(&lock, NULL);	
	pthread_cond_init(&cond, NULL);

	pthread_t threads[n_threads];
	struct data param[n_threads];
	int start=2, stop, tests, step;

	stop = ceil(num/2);
	tests = stop-start;
	step = ceil(tests/n_threads);

	clock_t begin_exec = clock();			// start tracking execution time

	for(i=0; i<n_threads; i++) {

		param[i].start = start;				// evaluate parameters and create threads
		param[i].stop = start + step;

		if (pthread_create(&threads[i], NULL, thread_func, (void *)&param[i]) != 0) {
			printf("Error generating thread %d. Exiting program...", i);
			exit(1);
		}

		start += step;
	} 

	while(divisible == 0 && prime != n_threads) {			// condition to end the program
		pthread_cond_wait(&cond, &lock);				// wait for a signal to test the condition variable
	}

	for (i=0; i<n_threads; i++) {
		pthread_cancel (threads[i]);				// end all active threads if needed
	}
	
	if(divisible != 0) {
		printf("Number %lu is divisible by: %d\n", num, divisible);
	}
	else if(prime == n_threads) {
		printf("Number %lu is prime\n", num);
	}

	clock_t end_exec = clock();					// stop tracking execution time
	double exec_time = (double)(end_exec - begin_exec) / CLOCKS_PER_SEC;
	printf("Execution time: %lf\n", exec_time);

	// destroy mutex and condition variable
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);

	return 0;
}