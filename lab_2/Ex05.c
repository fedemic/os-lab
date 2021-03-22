#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 6 	// number of children
#define N_infinite 3 // number of children that never terminates and print one random num at a time
#define N_RAND 5 	// random numbers generated in the last three children
#define MAX_RAND 100	// upper bound of random generation range

int main() {

	int i, j, status;
	pid_t pid[N];
	time_t t;
	pid[0] = -1;

	while(1) {
		if (pid[0] == -1) {
			for(i=0; i<N_infinite; i++) {	// first three children that never terminate
				pid[i] = fork();
			
				if(pid[i] == 0) {
					while(1) {
						srand((unsigned) time(&t) + getpid()); // PID + time used as seed for random generation
						int r = rand() % MAX_RAND;
						printf("CHILD %d (PID = %d): %d\n", i, getpid(), r);
						sleep(5);
					}
				}
			}
		}

		for(i=N_infinite; i<N; i++) {	// last three children that terminate
			pid[i] = fork();

			if(pid[i] == 0) {
				int r[N_RAND];
				printf("CHILD %d (PID = %d): ", i, getpid());

				for(j=0; j<N_RAND; j++) {		// generate N_RAND random numbers
					srand(getpid());			// only PID used as seed since these children terminate (new PID every iteration)
					r[j] = rand() % MAX_RAND;
					printf("%d ", r[j]*r[j]);
				}

				printf("\n");
				exit(0);				// when all number have been printed, terminate process
			}
		}

		for(i=N_infinite; i<N; i++) {		// when all last three children have terminated restart
			waitpid(pid[i], &status, 0);
		}
		sleep(5);			// slow down program
	}
	return 0;
}