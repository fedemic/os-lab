#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_FILENAME 30
#define MAX_OPERATIONS 100

sem_t sem;		// semaphore to handle CS
char filename[MAX_FILENAME];	// global file

void * thread_func() {

	FILE *f;
	int last_num, i;

	for(i=0; i<MAX_OPERATIONS; i++) {
		
		sem_wait(&sem); 	//semaphore wait to enter critical section

		//CS begin
		f = fopen(filename, "r");		// open file in read mode
		while(!feof(f)) {
			fscanf(f, "%d\n", &last_num);		// always replace read number until last line is reached
		}

		fclose(f);			//close file in read mode

		last_num += 3; 		// update variable to be written

		f = fopen(filename, "a");
		fprintf(f, "%d\n", last_num);
		fclose(f);
		// CS ends

		sem_post(&sem);
	}

	pthread_exit(NULL);
}


int main() {

	int n, i;
	FILE *f;

	printf("Insert filename: ");
	scanf("%s", filename);
	printf("Insert N: ");
	scanf("%d", &n);

	pthread_t threads[n];
	sem_init(&sem, 0, 1);		// semaphore initialization

	f = fopen(filename, "w");		// file creation
	fprintf(f, "%d\n", 3); 			// initialization of the first row
	fclose(f);

	for(i=0; i<n; i++) {
		if(pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {		// threads creation
			printf("Error generating thread %d. Exiting...\n", i);
		}
	}

	for(i=0; i<n; i++) {
		pthread_join(threads[i], NULL);		// threads join
	}

	sem_destroy(&sem);

	return 0;

}
