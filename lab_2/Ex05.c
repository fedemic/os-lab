#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 6 	// number of children
#define N_RAND 5 	// random numbers generated in the last three children
#define MAX_RAND 100	// mupper bound of random generation range

int main() {

	int i, j, status;
	pid_t pid[N];

	while(1) {
		for(i=0; i<N; i++) {
			pid[i] = fork();
			
			if(pid[i] == 0 && i <= 2) { // first three children
				srand(getpid());		// PID used as seed for random generation
				int r = rand() % MAX_RAND;
				printf("CHILD %d (PID = %d): %d\n", i, getpid(), r);
				exit(0);
			}

			else if(pid[i] == 0 && i >= 3) {	// last three children
				int r[N_RAND];
				printf("CHILD %d (PID = %d): ", i, getpid());

				for(j=0; j<N_RAND; j++) {		// generate N_RAND random numbers
					srand(getpid());
					r[j] = rand() % MAX_RAND;
					printf("%d ", r[j]*r[j]);
				}

				printf("\n");
				exit(0);
			}
		}

		for(i=0; i<N; i++) {		// when all children have terminated restart
			waitpid(pid[i], &status, 0);
		}
		sleep(5);
	}
	return 0;
}