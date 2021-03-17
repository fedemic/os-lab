#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

	pid_t pid1, pid2, pid3;

	pid1 = fork();

	if(pid1 == 0) { // child process
		printf("Process (PID = %d), Parent was with PID = %d\n", getpid(), getppid());

		pid2 = fork();

		if(pid2 == 0) {	// child of the child
			printf("Process (PID = %d), Parent was with PID = %d\n", getpid(), getppid());

			pid3 = fork();

			if(pid3 == 0) {	// child of the previous child
				printf("Process (PID = %d), Parent was with PID = %d\n", getpid(), getppid());
			}
		}

	}
	return 0;
}