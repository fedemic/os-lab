#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WRITE 1
#define READ 0
#define N 10

int main() {

	int i, status, num;
	float data_read=0, sum=0;

	printf("Enter n (between 2 and 10): ");
	scanf("%d", &num);
	if(num > 10) {
		printf("Replacing your n with 10...\n");
		num = N;
	}
	else if(num < 2) {
		printf("Replacing your n with 2...\n");
		num = 2;
	}

	int fd[num-1][2];
	pid_t pid[num-1];

	for(i=1; i<num; i++) {
		pipe(fd[i]);
		pid[i] = fork();

		if(pid[i] == 0) {		// child process
			float result = 2.79;
			for(int j=1; j<i; j++) {
				result *= 2.79;
			}
			result /= i;
			printf("CHILD %d: %.2f\n", i, result);

			write(fd[i][WRITE], &result, sizeof(result));
			close(fd[i][WRITE]);
			exit(0);
		}
	}

	// parent process
	for(i=1; i<num; i++) {
		waitpid(pid[i], &status, 0);
		read(fd[i][READ], &data_read, sizeof(data_read));
		close(fd[i][READ]);
		sum += data_read;
	}

	printf("Total sum is: %.2f\n", sum);
	return 0;
}
