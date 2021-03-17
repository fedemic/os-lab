#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

	int status;
	pid_t pid1, pid2, child_pid;

	pid1 = fork();

	if(pid1 < 0) // error
		printf("Error creating child process\n");

	else if(pid1 == 0) { // first child process
		printf("First child with PID = %d sleeping for 5 seconds\n", getpid());
		sleep(5); 	// wait for 5 seconds
		exit(2);
	}

	else {		// parent process
		pid2 = fork();

		if(pid2 == 0) { // second child
			printf("Second child with PID = %d sleeping for 5 seconds\n", getpid());
			sleep(5);
			exit(4);
		}

		else {	// parent process
			child_pid = 1;
			while(child_pid > 0) {		// until there are children alive wait for them
				child_pid = wait(&status);
				if(child_pid > 0) {
					printf("Child (PID = %d) terminated with status: %d\n", child_pid, WEXITSTATUS(status));
				}
			}
		}
	}
	return 0;
}