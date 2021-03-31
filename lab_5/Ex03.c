#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define N_CHILD 10
#define MAX_RAND 10

void alarmHandler() {
	printf("Child with pid = %d is still alive\n", getpid());

	srand(getpid()); 					// PID used as seed for random generation
	int r = rand() % MAX_RAND;

	alarm(r);
}

int main() {

	pid_t pid[N_CHILD];
	int i;
	int count = 0, choice;

	for(i=0; i<N_CHILD; i++) {
		pid[i] = fork();

		if(pid[i] == 0) {		// child process
			signal(SIGALRM, alarmHandler);		// handle alarms with proper function

			alarm(10);				// initial alarm set to 10 seconds
			while(1) {
			}
		}
	}

	while(count != N_CHILD) {
		printf("Choose a child to kill between the following:\n");

		for(i=0; i<N_CHILD; i++) {
			if(pid[i] != -1) {
				printf("%d\n", pid[i]);
			}
		}

		printf("Choice: ");
		scanf("%d", &choice);

		for(i=0; i<N_CHILD; i++) {
			if(pid[i] == choice) {
				printf("Terminating child with id = %d\n", choice);
				kill(pid[i], SIGQUIT);
				count++;
				pid[i] = -1;
			}
		}
	}
	printf("All children have been terminated.\n");
	return 0;
}