#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

	pid_t pid1, pid2, pid3;

	pid1 = fork();

	if(pid1 != 0) { // parent process
		printf("PARENT (PID = %d), CHILD generated with PID = %d\n", getpid(), pid1);

		pid2 = fork();

		if(pid2 != 0) {	// parent
			printf("PARENT (PID = %d), CHILD generated with PID = %d\n", getpid(), pid2);

			pid3 = fork();

			if(pid3 != 0) {	// parent
				printf("PARENT (PID = %d), CHILD generated with PID = %d\n", getpid(), pid3);
			}
		}

	}
	return 0;
}