#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

	int num, i, status, sum;

	printf("Enter an integer (max allowed is 5):\n");
	scanf("%d", &num);

	if(num > 5) {
		printf("Replacing your number with 5...\n");
		num = 5;
	}

	pid_t pid[num+1];

	for(i=0; i<num+1; i++) {
		pid[i] = fork();

		if(pid[i] == 0) {	// child process
			int result = 1;
			for(int j=0; j<i; j++) {
				result *= 2;
			}
			exit(result);
		}
	}

	for(i=0; i<num+1; i++) {
		waitpid(pid[i], &status, 0);		// wait for all children to terminate
		if(WIFEXITED(status)) {		// check if child has terminated correctly
			sum += WEXITSTATUS(status);		// accumulate partial sum
		}
	}

	printf("Final result for n = %d is: %d\n", num, sum);
	return 0;
}