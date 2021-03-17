#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

	pid_t pid;

	pid = fork();

	if(pid < 0) // error
		printf("Error creating child process\n");

	else if(pid == 0) // child process
		printf("CHILD with PID = %d\n", getpid());

	else // parent process
		printf("PARENT (PID = %d), CHILD has PID = %d\n", getpid(), pid);

	return 0;
}