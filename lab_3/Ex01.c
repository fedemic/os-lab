#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N_RAND 100
#define MAX_RAND 100
#define N_SUBPROCESSES 3

int main() {

	FILE *f;
	pid_t pid, sub_pid[N_SUBPROCESSES];
	int i, r, status;

	pid = fork();
	if(pid < 0) {		// error
		printf("Error generating child process\n");
		exit(1);
	}

	else if(pid == 0) {		// child process
		f = fopen("random.txt", "w");
		
		if (f == NULL) {
			printf("Error opening file\n");
			exit(1);
		}

		for(i=0; i<N_RAND; i++) {
			srand(i * getpid()); // PID and i index used as seed for random generation
			r = rand() % MAX_RAND;
			fprintf(f, "%d ", r);
		}
		fclose(f);
		printf("File generated\n");
	}

	else {		// parent process
		waitpid(pid, &status, 0);

		for(i=0; i<N_SUBPROCESSES; i++) {
			sub_pid[i] = fork();

			if(sub_pid[i] == 0 && i == 0) {	// first child subprocess
				execl("/usr/bin/wc", "wc", "-m", "/home/federico/Desktop/os-lab/lab_3/random.txt", NULL);		// linux command
				printf("Failed execl\n");
				exit(1);
			}
			else if(sub_pid[i] == 0 && i == 1) {		// second child subprocess
				execl("/usr/bin/wc", "wc", "-w", "/home/federico/Desktop/os-lab/lab_3/random.txt", NULL);		// linux command
				printf("Failed execl\n");
				exit(1);
			}
			else if(sub_pid[i] == 0 && i == 2) {		// third child subprocess
				execl("/home/federico/Desktop/os-lab/lab_3/get_average", "get_average", NULL, "/home/federico/Desktop/os-lab/lab_3", NULL);		// call custom program
				printf("Failed execl\n");
				exit(1);
			}
		}

		for(i=0; i<N_SUBPROCESSES; i++) {		// wait for three children to terminate
			waitpid(sub_pid[i], &status, 0);
			exit(0);
		}
	}

	return 0;
}