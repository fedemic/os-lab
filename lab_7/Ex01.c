#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#define MAX_GENOME 50000			// genome max length
#define MAX_SEQUENCE 100			// sequence max length

char genome[MAX_GENOME];
char sequence[MAX_SEQUENCE];

struct data {			// input parameters for each thread
	int start;
	int stop;
};

void *thread_func(void * thread_arg) {			// thread operations
	struct data *mydata;
	mydata = (struct data *)thread_arg;			// read imput parameters

	int begin = mydata->start, i=0;
	char substring[strlen(sequence)+1];			//substring is the part of genome under test


	while(begin != mydata->stop) {			// until it is the genome range related to this thread

		while(i < strlen(sequence)) {		// create substring
			substring[i] = genome[begin+i];
			i++;
		}

		if(strcmp(substring, sequence) == 0) {
			printf("Sequence found: start %d stop %d\n", begin, begin+strlen(sequence));		// output string
		}

		begin++;		// shift test window
		i=0;			// reset index
	}
}

int main() {

	char genome_filename[20], sequence_filename[20];
	FILE *f_gen, *f_seq;
	int n_threads, i;

	// user input parameters
	printf("Insert genome filename: ");
	scanf("%s", genome_filename);

	printf("Insert sequence filename: ");
	scanf("%s", sequence_filename);

	f_gen = fopen(genome_filename, "r");
	if (f_gen == NULL) {
		printf("Error opening genome file. Exiting program...\n");
		exit(1);
	}
	fscanf(f_gen, "%s", genome);
	fclose(f_gen);

	f_seq = fopen(sequence_filename, "r");
	if (f_seq == NULL) {
		printf("Error opening sequence file. Exiting program...\n");
		exit(1);
	}
	fscanf(f_seq, "%s", sequence);
	fclose(f_seq);

	// positive number of threads allowed
	do {
		printf("Insert number of desired threads: ");
		scanf("%d", &n_threads);
	} while (n_threads <= 0);


	pthread_t threads[n_threads];
	struct data myparam[n_threads];
	int start=0, stop, tests, step;
 
	tests = strlen(genome) - strlen(sequence);		// define parameters for algorithm
	step = ceil(tests/n_threads);

	for (i=0; i<n_threads; i++) {		// generate threads

		myparam[i].start = start;			// assign parameters to each thread
		myparam[i].stop = start + step;

		if (pthread_create(&threads[i], NULL, thread_func, (void *)&myparam[i]) != 0) {
			printf("Error creating thread %d. Exiting...\n", i);
			exit(1);
		}
		else {
			printf("Thread %d generated.\n", i);
		}

		start += step;				// update initial character for each thread

	}

	for (i=0; i<n_threads; i++) {
		pthread_join(threads[i], NULL);		// wait for every generated thread to finish their task
	}

	return 0;
}